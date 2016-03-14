#include "ssp.h"

#include <hardware.h>

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

static __inline__ uint16_t compute_prescaler(uint32_t clock) {
    if (clock >= SystemCoreClock / 2) {
        return SPI_BaudRatePrescaler_2;
    } else if (clock >= SystemCoreClock / 4) {
        return SPI_BaudRatePrescaler_4;
    } else if (clock >= SystemCoreClock / 8) {
        return SPI_BaudRatePrescaler_8;
    } else if (clock >= SystemCoreClock / 16) {
        return SPI_BaudRatePrescaler_16;
    } else if (clock >= SystemCoreClock / 32) {
        return SPI_BaudRatePrescaler_32;
    } else if (clock >= SystemCoreClock / 64) {
        return SPI_BaudRatePrescaler_64;
    } else if (clock >= SystemCoreClock / 128) {
        return SPI_BaudRatePrescaler_128;
    }

    return SPI_BaudRatePrescaler_256;
}

static SPI_TypeDef * const spis[] = { (void *) 0, SPI1, SPI2, SPI3 };

void ssp_config(ssp_port_t ssp_port, uint32_t clock)
{
    ssp_t ssp = ssp_port.ssp;
    pin_t sclk = ssp_port.sclk;
    pin_t miso = ssp_port.miso;
    pin_t mosi = ssp_port.mosi;

    SPI_TypeDef * id = spis[ssp];

    switch (ssp) {
    case ssp_port_1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        break;
    case ssp_port_2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        break;
    case ssp_port_3:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
        break;
    default:
        return;
    }

    uint32_t port_flags = 0;
    port_flags |= 1 << (sclk.port + 2);
    port_flags |= 1 << (miso.port + 2);
    port_flags |= 1 << (mosi.port + 2);
    RCC_APB2PeriphClockCmd(port_flags, ENABLE);


    GPIO_InitTypeDef gpiodef;
    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;
    gpiodef.GPIO_Mode = GPIO_Mode_AF_PP;

    gpiodef.GPIO_Pin = 1 << sclk.pin;
    GPIO_Init(stm32f10x_gpio_ports[sclk.port], &gpiodef);

    gpiodef.GPIO_Pin = 1 << mosi.pin;
    GPIO_Init(stm32f10x_gpio_ports[mosi.port], &gpiodef);

    gpiodef.GPIO_Pin = 1 << miso.pin;
    gpiodef.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(stm32f10x_gpio_ports[miso.port], &gpiodef);

    //clock AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    //Init SPI
    SPI_InitTypeDef spidef;
    spidef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spidef.SPI_Mode = SPI_Mode_Master;
    spidef.SPI_DataSize = SPI_DataSize_8b;
    spidef.SPI_CPOL = SPI_CPOL_Low;
    spidef.SPI_CPHA = SPI_CPHA_1Edge;
    spidef.SPI_NSS = SPI_NSS_Soft;
    spidef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    spidef.SPI_FirstBit = SPI_FirstBit_MSB;
    spidef.SPI_CRCPolynomial = 7;

    SPI_Init(id, &spidef);
    SPI_Cmd(id, ENABLE);
}

uint8_t ssp_readwrite(ssp_t ssp, uint8_t value)
{
    SPI_TypeDef * id = spis[ssp];

    //send data
    while (SPI_I2S_GetFlagStatus(id, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(id, value);

    //read data
    while (SPI_I2S_GetFlagStatus(id, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(id);
}
