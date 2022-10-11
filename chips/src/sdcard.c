#include <BoardConsole.h>

#include "gpio.h"
#include "ssp.h"
#include "sdcard.h"


#ifdef FULLDEBUG
#define MAX_RETRIES 5
#define SDPRINTF BoardConsolePrintf
#define SDPUTS BoardConsolePuts
#else
#define MAX_RETRIES 10000
#define SDPRINTF(...)
#define SDPUTS(x)
#endif

/*
 * The SDCard specifications are out of whack. Seriously. I've used a bunch of ressources to write
 * that source code. Here's a list of references I used:
 *
 * http://users.ece.utexas.edu/~valvano/EE345M/SD_Physical_Layer_Spec.pdf
 * http://alumni.cs.ucr.edu/~amitra/sdcard/Additional/sdcard_appnote_foust.pdf
 * http://elm-chan.org/docs/mmc/mmc_e.html
 * http://wiki.seabright.co.nz/wiki/SdCardProtocol.html
 *
 * Now, funnily enough, most of these informations (plus various other sources codes I've read)
 * contain contradictory information. So at the end, I'm not really sure that code fully works
 * everywhere. If you end up using that code, and stumble upon an sdcard that doesn't work, please
 * let me know and potentially send me said sdcard, so I can correct the code.
 *
 */

enum {
    GO_IDLE_STATE = 0,
    SEND_OP_COND = 1,
    ALL_SEND_CID = 2,
    SEND_RELATIVE_ADDR,
    SEND_IF_COND = 8,
    SEND_CSD = 9,
    SEND_CID = 10,
    STOP_TRANSMISSION = 12,
    SET_BLOCKLEN = 16,
    READ_SINGLE_BLOCK = 17,
    READ_MULTIPLE_BLOCK = 18,
    SET_BLOCK_COUNT = 23,
    WRITE_BLOCK = 24,
    WRITE_MULTIPLE_BLOCK = 25,
    READ_OCR = 58,
    CRC_ON_OFF = 59,

    APP_CMD = 55,

    APP_SEND_OP_COND = 41,
    SET_WR_BLOCK_ERASE_COUNT = 23,

    R1 = 0,
    R1B = 1,
    R2 = 2,
    R3 = 5,

    IDLE_STATE = 1 << 0,
    ERASE_RESET = 1 << 1,
    ILLEGAL_COMMAND = 1 << 2,
    COMMAND_CRC_ERROR = 1 << 3,
    ERASE_SEQUENCE_ERROR = 1 << 4,
    ADDRESS_ERROR = 1 << 5,
    PARAMETER_ERROR = 1 << 6,
};

static uint8_t update_cmd_crc(uint8_t crc, uint8_t val) {
    unsigned int i;
    int bit;

    for (i = 0x80; i > 0; i >>= 1) {
        bit = (crc & 0x40);
        if (val & i)
            bit ^= 0x40;
        crc <<= 1;
        if (bit)
            crc ^= 9;
    }

    return crc & 0x7f;
}

static const uint16_t crc16_table[16] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef
};

static __inline__ uint16_t update_data_crc(uint16_t crc, uint8_t val) {
    unsigned int tbl_idx;

    tbl_idx = (crc >> 12) ^ (val >> 4);
    crc = crc16_table[tbl_idx & 0x0f] ^ (crc << 4);
    tbl_idx = (crc >> 12) ^ (val >> 0);
    crc = crc16_table[tbl_idx & 0x0f] ^ (crc << 4);

    return crc;
}

static int sdcard_cmd(sdcard_t * sdcard, uint8_t cmd, uint8_t arg[4], int big_response, int wait, uint8_t response[17]) {
    SDPRINTF("  sending cmd %02x\n", cmd);
    cmd &= 0x3f;
    cmd |= 0x40;
    gpio_set(sdcard->cs, 0);
    ssp_write(sdcard->ssp, cmd);
    uint8_t crc = update_cmd_crc(0, cmd);
    int i;
    for (i = 0; i < 4; i++) {
        crc = update_cmd_crc(crc, arg[i]);
        ssp_write(sdcard->ssp, arg[i]);
    }
    ssp_write(sdcard->ssp, (crc << 1) | 1);

    int retries = MAX_RETRIES;
    while (--retries) {
        response[0] = ssp_read(sdcard->ssp);
        if ((response[0] & 0x80) == 0)
            break;
        if (response[0] != 0xff)
            SDPRINTF("Got odd idle byte: %02x\n", response[0]);
    }

    if (retries == 0) {
        SDPRINTF("Timeout while waiting for response (got %02x).\n", response[0]);
        sdcard->got_timeout = 1;
        gpio_set(sdcard->cs, 1);
        return 0;
    }

    if (big_response)
        big_response = 17;
    else
        big_response = 6;

    for (i = 1; i < big_response; i++)
        response[i] = ssp_read(sdcard->ssp);

    if (wait) {
        SDPUTS("Waiting...");
        retries = MAX_RETRIES;
        while (--retries)
            (void) ssp_read(sdcard->ssp);
        if (retries == 0) {
            sdcard->got_timeout = 1;
            gpio_set(sdcard->cs, 1);
            return 0;
        }
        ssp_write(sdcard->ssp, 0xff);
    }

    gpio_set(sdcard->cs, 1);
    return 1;
}

static void error_out(sdcard_t * sdcard) {
    gpio_set(sdcard->cs, 1);
    ssp_write(sdcard->ssp, 0xff);
    sdcard->error_state = 1;
}

int sdcard_init(sdcard_t * sdcard, ssp_port_t ssp_port, pin_t cs) {
    int i;

    SDPUTS("Starting sdcard init..");

    sdcard->cs = cs;
    sdcard->ssp = ssp_port.ssp;
    sdcard->got_timeout = 0;
    sdcard->error_state = 0;
    sdcard->v2 = 0;
    sdcard->sdhc = 0;

    SDPUTS("Configuring SSP.");
    ssp_config(ssp_port, 400000); // standard says we can ping the card at 400khz first.
    SDPUTS("Configuring CS.");
    gpio_config(sdcard->cs, pin_dir_write, pull_up);
    gpio_set(sdcard->cs, 1);

    // first, let's send 74 dummy clocks, with MOSI = 1 and CS = 1; that's about 10 bytes equal to 0xff.
    SDPUTS("Waiting 74+ cycles.");
    for (i = 0; i < 10; i++)
        ssp_write(sdcard->ssp, 0xff);

    SDPUTS("Waiting 16+ cycles.");
    // then, we need 16 dummy clocks, with MOSI = 1 and CS = 0. Two bytes.
    gpio_set(sdcard->cs, 0);
    for (i = 0; i < 2; i++)
        ssp_write(sdcard->ssp, 0xff);

    uint8_t argument[4] = { 0, 0, 0, 0 };
    uint8_t response[17];

    // the card ought to be ready for us to send things now; let's start with tuning on SPI mode
    SDPUTS("Sending GO_IDLE_STATE command.");
    if (!sdcard_cmd(sdcard, GO_IDLE_STATE, argument, 0, 0, response)) {
        SDPUTS("  --> timeouted.");
        error_out(sdcard);
        return 0;
    }
    SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);
    if (response[0] != IDLE_STATE) {
        SDPUTS("Not in idle state...\n");
        error_out(sdcard);
        return 0;
    }

    // sending that will a) tell us if this is a v2 card and b) enable the card for getting the special SDHC commands, if supported.
    SDPUTS("Sending SEND_IF_COND command to see if this is a v2 card.");
    argument[0] = 0;
    argument[1] = 0;
    argument[2] = 1;
    argument[3] = 0xaa;
    if (!sdcard_cmd(sdcard, SEND_IF_COND, argument, 0, 0, response)) {
        SDPUTS("  --> timeouted.");
        error_out(sdcard);
        return 0;
    }

    SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);
    if ((response[0] & IDLE_STATE) != IDLE_STATE) {
        SDPUTS("Not in idle state...\n");
        error_out(sdcard);
        return 0;
    }

    if ((response[0] & ~ILLEGAL_COMMAND) != IDLE_STATE) {
        SDPUTS("Got an unknown error...\n");
        error_out(sdcard);
        return 0;
    }

    if ((response[0] & ILLEGAL_COMMAND) != ILLEGAL_COMMAND) {
        // The card accepted the command, so it's a v2.
        SDPUTS("Card is a v2");
        sdcard->v2 = 1;
    }

    int retries = MAX_RETRIES;

    SDPUTS("Initializing the SDCard by sending APP_SEND_OP_COND");
    while (--retries) {
        argument[0] = 0;
        argument[1] = 0;
        argument[2] = 0;
        argument[3] = 0;
        // We need to send APP_SEND_OP_COND, which needs APP_CMD before.
        SDPUTS("Sending APP_CMD.");
        if (!sdcard_cmd(sdcard, APP_CMD, argument, 0, 0, response)) {
            SDPUTS("  --> timeouted.");
            error_out(sdcard);
            return 0;
        }
        SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);
        argument[0] = sdcard->v2 ? 0x40 : 0;
        argument[1] = 0;
        argument[2] = 0;
        argument[3] = 0;
        SDPUTS("Sending APP_SEND_OP_COND.");
        if (!sdcard_cmd(sdcard, APP_SEND_OP_COND, argument, 0, 0, response)) {
            SDPUTS("  --> timeouted.");
            error_out(sdcard);
            return 0;
        }
        SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);
        // if bit 0 is set, card is busy, so we need to loop again
        if ((response[0] & IDLE_STATE) == 0)
            break;
    }

    if (retries == 0) {
        SDPUTS("Gave up initializing the sdcard");
        sdcard->got_timeout = 1;
        error_out(sdcard);
        return 0;
    }

    if (response[0] != 0) {
        SDPUTS("Bad response to APP_SEND_OP_COND");
        error_out(sdcard);
        return 0;
    }

    // We're in fact supposed to retry using the SEND_OP_COND message instead of erroring out, but I'd
    // like to get an actual sdcard with these requirements to properly test it. In the meantime, I'll
    // just error out. It seems to be the "MMC V3" sdcard btw.

    ssp_config(ssp_port, 4000000); // let's crank up the speed to 4Mhz.

    SDPUTS("Turning on CRC checks");
    // since we're computing them, might as well require the card to check them...
    if (!sdcard_cmd(sdcard, CRC_ON_OFF, argument, 0, 0, response)) {
        SDPUTS("  --> timeouted.");
        error_out(sdcard);
        return 0;
    }
    SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);

    if (response[0] != 0) {
        SDPUTS("Bad response to CRC_ON_OFF");
        error_out(sdcard);
        return 0;
    }

    if (sdcard->v2) { // not sure about that one
        // check the OCR information now
        SDPUTS("Sending READ_OCR");
        argument[0] = 0;
        argument[1] = 0;
        argument[2] = 0;
        argument[3] = 0;
        if (!sdcard_cmd(sdcard, READ_OCR, argument, 0, 0, response)) {
            SDPUTS("  --> timeouted.");
            error_out(sdcard);
            return 0;
        }
        SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);

        if (response[0] != 0) {
            SDPUTS("Bad response to READ_OCR");
            error_out(sdcard);
            return 0;
        }

        sdcard->sdhc = response[1] & 0x40 ? 1 : 0;
    }

    if (!sdcard->sdhc) {
        SDPUTS("Sending SET_BLOCKLEN with arg = 512");
        argument[0] = 0;
        argument[1] = 0;
        argument[2] = 2;
        argument[3] = 0;
        if (!sdcard_cmd(sdcard, SET_BLOCKLEN, argument, 0, 0, response)) {
            SDPUTS("  --> timeouted.");
            error_out(sdcard);
            return 0;
        }
        SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);

        if (response[0] != 0) {
            SDPUTS("Bad response to READ_OCR");
            error_out(sdcard);
            return 0;
        }
    }

    SDPRINTF("SDCard init success. v2 = %s, sdhc = %s\n", sdcard->v2 ? "true" : "false", sdcard->sdhc ? "true" : "false");

    return 1;
}

int sdcard_read(sdcard_t * sdcard, uint8_t * data, unsigned int block) {
    uint8_t argument[4] = { 0, 0, 0, 0 };
    uint8_t response[17];
    uint32_t offset = block;
    if (sdcard->sdhc)
        offset *= 512;

    argument[3] = offset & 0xff; offset >>= 8;
    argument[2] = offset & 0xff; offset >>= 8;
    argument[1] = offset & 0xff; offset >>= 8;
    argument[0] = offset & 0xff; offset >>= 8;
    if (!sdcard_cmd(sdcard, READ_SINGLE_BLOCK, argument, 0, 0, response)) {
        SDPUTS("  --> timeouted.");
        error_out(sdcard);
        return 0;
    }
    SDPRINTF("  --> %02x %02x %02x %02x %02x %02x\n", response[0], response[1], response[2], response[3], response[4], response[5]);

    gpio_set(sdcard->cs, 0);
    int retries = MAX_RETRIES;
    while (--retries) {
        uint8_t pad = ssp_read(sdcard->ssp);
        if (pad == 0xff)
            continue;
        if (pad == 0xfe)
            break;
        SDPUTS("  --> invalid padding.");
        error_out(sdcard);
        return 0;
    }

    if (retries == 0) {
        SDPUTS("Gave up waiting sdcard's sector");
        sdcard->got_timeout = 1;
        error_out(sdcard);
        return 0;
    }

    uint16_t crc = 0;
    int i;

    for (i = 0; i < 512; i++) {
        uint8_t val = data[i] = ssp_read(sdcard->ssp);
        crc = update_data_crc(crc, val);
    }

    uint16_t sdcrc = ssp_read(sdcard->ssp);
    sdcrc <<= 8;
    sdcrc |= ssp_read(sdcard->ssp);

    gpio_set(sdcard->cs, 1);

    SDPRINTF("Read crc = %04x, computed crc = %04x\n", sdcrc, crc);

    return sdcrc == crc ? 1 : 0;
}
