#include "gpio.h"
#include "ssp.h"
#include "lis3dsh.h"

// http://www.st.com/web/en/resource/technical/document/datasheet/DM00040962.pdf
// Disclaimer: that above documentation is abysmally wrong. Several pages of
// them even, so it may or may not be a copy/paste mistake. The SPI protocol
// this chip follows isn't the one described at all. More specifically, there is
// no such thing as a "multibyte" flag in the register address byte. If you set
// it, you'll actually end up reading or writing registers from 0x40 to 0x7f
// instead of doing multibytes operations on 0x00 to 0x3f. Instead, multibyte is
// controlled by the flag ADD_INC from the control register 6. It seems this
// flag is set to enabled by default, but we'll probably want to set it anyway,
// just to be sure.

static __inline__ void spi_read_registers(ssp_t ssp, uint8_t address, uint8_t * buffer, uint8_t size) {
    ssp_write(ssp, address | 0x80);
    while (size--) *buffer++ = ssp_read(ssp);
}

static __inline__ void spi_write_registers(ssp_t ssp, uint8_t address, const uint8_t * buffer, uint8_t size) {
    ssp_write(ssp, address);
    while (size--) ssp_write(ssp, *buffer++);
}

int lis3dsh_init_ssp(lis3dsh_t * lis3dsh, ssp_port_t ssp_port) {
    pin_t cs = lis3dsh->cs;
    ssp_t ssp = get_ssp(ssp_port);
    lis3dsh->ssp = ssp;

    gpio_config(cs, pin_dir_write, pull_up);
    gpio_set(cs, 1);

    // lis3dsh can talk at 10Mhz according to the docs, but I got
    // jitter at that speed. So 4Mhz it is for the moment.
    ssp_config(ssp_port, 4 * 1000 * 1000);
    ssp_write(ssp, 0xff);

    uint8_t b;

    // Let's read WHO_AM_I (0x0f) - should be 0x3f for LIS3DSH.
    gpio_set(cs, 0);
    spi_read_registers(ssp, 0x0f, &b, 1);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);
    if (b != 0x3f) return 0;

    // Let's write to CTRL_REG4 (0x20)
    // 0x67 = 01100111
    // bit0: Enable X
    // bit1: Enable Y
    // bit2: Enable Z
    // bit3: Continuous update
    // bit4-bit7: ODR = 100Hz - sample frequency
    b = 0x67;
    lis3dsh->odr = 6;
    gpio_set(cs, 0);
    spi_write_registers(ssp, 0x20, &b, 1);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    // Let's write to CTRL_REG5 (0x24)
    // Basically, setting defaults, just in case.
    // SPI mode: 4 wires
    // Self test: disabled
    // Full scale: 2G
    // Anti-aliasing filter bandwidth: 800Hz
    b = 0x00;
    lis3dsh->scale = 0;
    gpio_set(cs, 0);
    spi_write_registers(ssp, 0x24, &b, 1);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    return 1;
}

void lis3dsh_power(lis3dsh_t * lis3dsh, int power) {
    pin_t cs = lis3dsh->cs;
    ssp_t ssp = lis3dsh->ssp;

    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    uint8_t b;

    // power off mode = 0x07
    // power on mode  = 0x67

    b = 7;
    if (power) {
        b |= lis3dsh->odr << 4;
    }
    gpio_set(cs, 0);
    spi_write_registers(ssp, 0x20, &b, 1);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);
}

void lis3dsh_read(lis3dsh_t * lis3dsh, float axis[3]) {
    pin_t cs = lis3dsh->cs;
    ssp_t ssp = lis3dsh->ssp;

    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    uint8_t registers[6];
    gpio_set(cs, 0);
    spi_read_registers(ssp, 0x28, registers, 6);
    gpio_set(cs, 1);
    ssp_write(ssp, 0xff);

    float sensitivity = 0.0f;

    // Reading scaling register from CTRL_REG5 (0x24)
    // That's bits 3 to 5.
    switch (lis3dsh->scale) {
    case 0:
        sensitivity = 0.06f;
        break;
    case 1:
        sensitivity = 0.12f;
        break;
    case 2:
        sensitivity = 0.18f;
        break;
    case 3:
        sensitivity = 0.24f;
        break;
    case 4:
        sensitivity = 0.73f;
        break;
    }

    int i;
    // Now let's read X, Y, and Z.
    // That's registers 0x28 to 0x2d.
    // X is in 0x28 - 0x29.
    // Y is in 0x2a - 0x2b.
    // Z is in 0x2c - 0x2d.
    // The data is in Little Endian.
#ifndef TARGET_LITTLE_ENDIAN
    for (i = 0; i < 3; i++) {
        uint8_t t;
        t = registers[i * 2 + 5];
        registers[i * 2 + 5] = registers[i * 2 + 4];
        registers[i * 2 + 4] = t;
    }
#endif
    // Yes, I know, pointer aliasing. But the whole code is filled with it
    // anyway, and that should be the least of our problems.
    int16_t * axis_data = (int16_t *) (registers + 4);
    for (i = 0; i < 3; i++) {
        axis[i] = sensitivity * axis_data[i];
    }
}
