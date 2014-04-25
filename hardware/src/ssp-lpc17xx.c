#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include <lpc17xx_clkpwr.h>
#include <lpc17xx_ssp.h>

#include "gpio.h"
#include "ssp.h"

void ssp_config(ssp_t ssp, uint32_t clock) {
    SSP_CFG_Type cfg;
    LPC_SSP_TypeDef * sspdef = NULL;
    int sclk, miso, mosi;
    uint32_t clkpwr;
    int port = 0;

    switch (ssp) {
    case ssp_port_0:
        clkpwr = CLKPWR_PCLKSEL_SSP0;
        sspdef = LPC_SSP0;
        sclk = 15;
        miso = 17;
        mosi = 18;
        break;
    case ssp_port_1:
        clkpwr = CLKPWR_PCLKSEL_SSP1;
        sspdef = LPC_SSP1;
        sclk = 7;
        miso = 8;
        mosi = 9;
        break;
    default:
        return;
    }

    PINSEL_CFG_Type pin_cfg;
    pin_cfg.Portnum = port;
    pin_cfg.Funcnum = 2;
    pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
    pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;

    pin_cfg.Pinnum = sclk; PINSEL_ConfigPin(&pin_cfg);
    pin_cfg.Pinnum = miso; PINSEL_ConfigPin(&pin_cfg);
    pin_cfg.Pinnum = mosi; PINSEL_ConfigPin(&pin_cfg);

    FIO_SetDir(port, 1 << sclk, 1);
    FIO_SetDir(port, 1 << miso, 0);
    FIO_SetDir(port, 1 << mosi, 1);

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
        case ssp_port_0: sspdef = LPC_SSP0; break;
        case ssp_port_1: sspdef = LPC_SSP1; break;
        default: return 0;
    }

    while ((sspdef->SR & SSP_SR_TNF) == 0);
    SSP_SendData(sspdef, value);
    while ((sspdef->SR & SSP_SR_RNE) == 0);
    return (uint8_t) SSP_ReceiveData(sspdef);
}
