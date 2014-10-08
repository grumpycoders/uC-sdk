#include "spi.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>

#include <stdio.h>

struct spiInitDef_t {
    // sck / mosi / miso / cs
    SPI_TypeDef * id;
    GPIO_TypeDef * tdef[4];
    GPIO_InitTypeDef gpiodef[4];
    // spi / gpio
    volatile uint32_t * bridge[2];
    uint32_t peripheral[2];
};

static struct spiInitDef_t spiInitDefs[3] = {
    { SPI1, { GPIOA, GPIOA, GPIOA, GPIOA}, {   // SPI1
        { GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // SCK
        { GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // MOSI
        { GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING }, // MISO
        { GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_Out_PP },      // CS
    }, {&RCC->APB2ENR, &RCC->APB2ENR}, {RCC_APB2Periph_SPI1, RCC_APB2Periph_GPIOA } },
    { SPI2, { GPIOB, GPIOB, GPIOB, GPIOB}, {   // SPI2
        { GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // SCK
        { GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // MOSI
        { GPIO_Pin_14, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING }, // MISO
        { GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_Out_PP },      // CS
    }, {&RCC->APB1ENR, &RCC->APB2ENR}, {RCC_APB1Periph_SPI2, RCC_APB2Periph_GPIOB }  },
    { SPI3, { GPIOB, GPIOB, GPIOB, GPIOA}, {   // SPI3
        { GPIO_Pin_3, GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // SCK
        { GPIO_Pin_5, GPIO_Speed_50MHz, GPIO_Mode_AF_PP },       // MOSI
        { GPIO_Pin_4, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING }, // MISO
        { GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_Out_PP },     // CS
    }, {&RCC->APB1ENR, &RCC->APB2ENR}, {RCC_APB1Periph_SPI3, RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB } },
};

void spi_init(uint8_t id)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;

    //clock AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    int i;
    for (i = 0; i < 2; i++)
        *(spiInitDef->bridge[i]) |= spiInitDef->peripheral[i];

    for (i = 0; i < 4; i++)
        GPIO_Init(spiInitDef->tdef[i], &spiInitDef->gpiodef[i]);

    //GPIO_SetBits(spiInitDef->tdef[3], spiInitDef->gpiodef[3].GPIO_Pin);
    spi_stop(id);

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

    SPI_Init(spiInitDef->id, &spidef); 
    SPI_Cmd(spiInitDef->id, ENABLE);
}

void spi_start(uint8_t id)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;
    GPIO_ResetBits(spiInitDef->tdef[3], spiInitDef->gpiodef[3].GPIO_Pin);
}

void spi_stop(uint8_t id)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;
    GPIO_SetBits(spiInitDef->tdef[3], spiInitDef->gpiodef[3].GPIO_Pin);
}

void spi_read(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;

    while(nb--)
    {
        taskENTER_CRITICAL();

        //send garbage to work the clock
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(spiInitDef->id, 0xFF);

        //read data
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_RXNE) == RESET);
        *buffer++ = (uint8_t) SPI_I2S_ReceiveData(spiInitDef->id);

        taskEXIT_CRITICAL();
    }
}

void spi_write(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    if (!((id >= 1) && (id <= 3)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;

    while(nb--)
    {
        taskENTER_CRITICAL();

        //send data
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(spiInitDef->id, *buffer++);

        //read, because that's the rule
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_RXNE) == RESET);
        SPI_I2S_ReceiveData(spiInitDef->id);

        taskEXIT_CRITICAL();
    }
}
