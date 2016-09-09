#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_clkpwr.h>
#include <lpc17xx_ssp.h>

#include "gpio.h"
#include "ssp.h"

void ssp_config(ssp_port_t ssp_port, uint32_t clock) {
    SSP_CFG_Type cfg;
    LPC_SSP_TypeDef * sspdef = NULL;
    uint32_t clkpwr;
    ssp_t ssp = ssp_port.ssp;
    pin_t sclk = ssp_port.sclk;
    pin_t miso = ssp_port.miso;
    pin_t mosi = ssp_port.mosi;

    switch (ssp) {
    case ssp_0:
        clkpwr = CLKPWR_PCLKSEL_SSP0;
        sspdef = LPC_SSP0;
        break;
    case ssp_1:
        clkpwr = CLKPWR_PCLKSEL_SSP1;
        sspdef = LPC_SSP1;
        break;
    default:
        return;
    }

    PINSEL_CFG_Type pin_cfg;
    pin_cfg.Funcnum = 2;
    pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
    pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    pin_cfg.Pinnum = sclk.pin;
    pin_cfg.Portnum = sclk.port;
    FIO_SetDir(sclk.port, 1 << sclk.pin, 1);
    PINSEL_ConfigPin(&pin_cfg);

    pin_cfg.Pinnum = miso.pin;
    pin_cfg.Portnum = miso.port;
    FIO_SetDir(miso.port, 1 << miso.pin, 0);
    PINSEL_ConfigPin(&pin_cfg);

    pin_cfg.Pinnum = mosi.pin;
    pin_cfg.Portnum = mosi.port;
    FIO_SetDir(mosi.port, 1 << mosi.pin, 1);
    PINSEL_ConfigPin(&pin_cfg);

    CLKPWR_SetPCLKDiv(clkpwr, CLKPWR_PCLKSEL_CCLK_DIV_1);

    SSP_ConfigStructInit(&cfg);
    cfg.ClockRate = clock;

    SSP_Init(sspdef, &cfg);
    SSP_Cmd(sspdef, ENABLE);
}

// There's code for that in NXP's CMSIS, but it's overkill.
uint8_t ssp_readwrite(ssp_t ssp, uint8_t value) {
    LPC_SSP_TypeDef * sspdef = NULL;

    switch (ssp) {
        case ssp_0: sspdef = LPC_SSP0; break;
        case ssp_1: sspdef = LPC_SSP1; break;
        default: return 0;
    }

    while ((sspdef->SR & SSP_SR_TNF) == 0);
    SSP_SendData(sspdef, value);
    while ((sspdef->SR & SSP_SR_RNE) == 0);
    return (uint8_t) SSP_ReceiveData(sspdef);
}
