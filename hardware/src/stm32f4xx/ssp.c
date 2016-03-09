#include "ssp.h"
#include <hardware.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>

static SPI_TypeDef * const spis[] = { (void *) 0, SPI1, SPI2, SPI3, SPI4, SPI5, SPI6 };

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

void ssp_config(ssp_port_t ssp_port, uint32_t clock) {
    ssp_t ssp = ssp_port.ssp;
    pin_t sclk = ssp_port.sclk;
    pin_t miso = ssp_port.miso;
    pin_t mosi = ssp_port.mosi;

    SPI_TypeDef * id = spis[ssp];
    uint8_t af;

    switch (ssp) {
    case ssp_port_1:
        af = GPIO_AF_SPI1;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        break;
    case ssp_port_2:
        af = GPIO_AF_SPI2;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        break;
    case ssp_port_3:
        af = GPIO_AF_SPI3;
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
        break;
    case ssp_port_4:
        af = GPIO_AF_SPI4;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
        break;
    case ssp_port_5:
        af = GPIO_AF_SPI5;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
        break;
    case ssp_port_6:
        af = GPIO_AF_SPI6;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);
        break;
    default:
        return;
    }

    uint32_t port_flags = 0;
    port_flags |= 1 << sclk.port;
    port_flags |= 1 << miso.port;
    port_flags |= 1 << mosi.port;
    RCC_AHB1PeriphClockCmd(port_flags, ENABLE);


    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin   = 1 << sclk.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[sclk.port], sclk.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[sclk.port], &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = 1 << miso.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[miso.port], miso.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[miso.port], &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = 1 << mosi.pin;
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[mosi.port], mosi.pin, af);
    GPIO_Init(stm32f4xx_gpio_ports[mosi.port], &GPIO_InitStructure);


    SPI_InitTypeDef SPI_InitStructure;

    SPI_I2S_DeInit(id);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = compute_prescaler(clock);
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(id, &SPI_InitStructure);

    SPI_Cmd(id, ENABLE);
}

uint8_t ssp_readwrite(ssp_t ssp, uint8_t value) {
    SPI_TypeDef * id = spis[ssp];

    while (SPI_I2S_GetFlagStatus(id, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(id, value);
    while (SPI_I2S_GetFlagStatus(id, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(id);
}
