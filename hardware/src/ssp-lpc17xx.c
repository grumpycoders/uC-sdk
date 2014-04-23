#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_ssp.h>

#include "gpio.h"
#include "ssp.h"

void ssp_config(ssp_t ssp, uint32_t clock) {
    SSP_CFG_Type cfg;
    LPC_SSP_TypeDef * sspdef = NULL;
    int sclk, miso, mosi;
    uint32_t mask_read, mask_write;

    switch (ssp) {
    case ssp_port_0:
        sspdef = LPC_SSP0;
        sclk = 15;
        miso = 17;
        mosi = 18;
        mask_read = 1 << 17;
        mask_write = (1 << 15) | (1 << 18);
        break;
    case ssp_port_1:
        sspdef = LPC_SSP1;
        sclk = 7;
        miso = 8;
        mosi = 9;
        mask_read = 1 << 8;
        mask_write = (1 << 7) | (1 << 9);
        break;
    default:
        return;
    }

    SSP_ConfigStructInit(&cfg);
    cfg.ClockRate = clock;

    PINSEL_CFG_Type pin_cfg;
    pin_cfg.Portnum = 0;
    pin_cfg.Funcnum = 2;
    pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
    pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    pin_cfg.Pinnum = sclk; PINSEL_ConfigPin(&pin_cfg);
    pin_cfg.Pinnum = miso; PINSEL_ConfigPin(&pin_cfg);
    pin_cfg.Pinnum = mosi; PINSEL_ConfigPin(&pin_cfg);

    FIO_SetDir(0, mask_read, 0);
    FIO_SetDir(0, mask_write, 1);

    SSP_Init(sspdef, &cfg);
}

// There's code for that in NXP's CMSIS, but it's overkill.
void ssp_write(ssp_t ssp, uint8_t value) {
    LPC_SSP_TypeDef * sspdef = NULL;

    switch (ssp) {
        case ssp_port_0: sspdef = LPC_SSP0; break;
        case ssp_port_1: sspdef = LPC_SSP1; break;
        default: return;
    }

    while ((sspdef->SR & SSP_SR_TNF) == 0);
    SSP_SendData(sspdef, value);
    while ((sspdef->SR & SSP_SR_RNE) == 0);
    (void) SSP_ReceiveData(sspdef);
}

uint8_t ssp_read(ssp_t ssp) {
    LPC_SSP_TypeDef * sspdef = NULL;

    switch (ssp) {
        case ssp_port_0: sspdef = LPC_SSP0; break;
        case ssp_port_1: sspdef = LPC_SSP1; break;
        default: return 0;
    }

    while ((sspdef->SR & SSP_SR_TNF) == 0);
    SSP_SendData(sspdef, 0xff);
    while ((sspdef->SR & SSP_SR_RNE) == 0);
    return (uint8_t) SSP_ReceiveData(sspdef);
}
