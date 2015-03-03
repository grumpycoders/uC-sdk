#include "spi.h"

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>

#include <stdio.h>


#define READ_BYTE 0x80
#define MULTIBYTE_BYTE 0x40

struct spiInitDef_t {
    // sck / mosi / miso / cs
    SPI_TypeDef * id;
    GPIO_TypeDef * tdef[4];
    GPIO_InitTypeDef gpiodef[4];
    uint16_t pinsource[4];
    // spi / gpio
    volatile uint32_t * bridge[2];
    uint32_t peripheral[2];
    uint8_t af;
};

//CS is probably useless: we use software
static struct spiInitDef_t spiInitDefs[6] = {
    { SPI1, { GPIOA, GPIOA, GPIOA, GPIOA}, {   // SPI1
        { GPIO_Pin_5, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // SCK
        { GPIO_Pin_7, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // MOSI
        { GPIO_Pin_6, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL },  // MISO
        { GPIO_Pin_4, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL }, // CS
      }, {GPIO_PinSource5, GPIO_PinSource7, GPIO_PinSource6, GPIO_PinSource4}, 
      {&RCC->APB2ENR, &RCC->AHB1ENR}, {RCC_APB2Periph_SPI1, RCC_AHB1Periph_GPIOA }, GPIO_AF_SPI1 },
    { SPI2, { GPIOB, GPIOB, GPIOB, GPIOB}, {   // SPI2
        { GPIO_Pin_10, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // SCK
        { GPIO_Pin_15, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // MOSI
        { GPIO_Pin_14, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL },  // MISO
        { GPIO_Pin_9, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL }, // CS
      }, {GPIO_PinSource10, GPIO_PinSource15, GPIO_PinSource14, GPIO_PinSource9}, 
      {&RCC->APB1ENR, &RCC->AHB1ENR}, {RCC_APB1Periph_SPI2, RCC_AHB1Periph_GPIOB }, GPIO_AF_SPI2 },
    { SPI3, { GPIOB, GPIOB, GPIOB, GPIOA}, {   // SPI3
        { GPIO_Pin_3, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // SCK
        { GPIO_Pin_5, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // MOSI
        { GPIO_Pin_4, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL },  // MISO
        { GPIO_Pin_4, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL }, // CS
      }, {GPIO_PinSource3, GPIO_PinSource5, GPIO_PinSource4, GPIO_PinSource4}, 
      {&RCC->APB1ENR, &RCC->AHB1ENR}, {RCC_APB1Periph_SPI3, RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB }, GPIO_AF_SPI3 },
    { SPI4, { GPIOE, GPIOE, GPIOE, GPIOE}, {   // SPI4
        { GPIO_Pin_2, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // SCK
        { GPIO_Pin_6, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // MOSI
        { GPIO_Pin_5, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL },  // MISO
        { GPIO_Pin_4, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL }, // CS
      }, {GPIO_PinSource2, GPIO_PinSource6, GPIO_PinSource5, GPIO_PinSource4}, 
      {&RCC->APB2ENR, &RCC->AHB1ENR}, {RCC_APB2Periph_SPI4, RCC_AHB1Periph_GPIOE }, GPIO_AF_SPI4 },
    { SPI5, { GPIOF, GPIOF, GPIOF, GPIOF}, {   // SPI5
        { GPIO_Pin_7, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_DOWN },  // SCK
        { GPIO_Pin_9, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_DOWN },  // MOSI
        { GPIO_Pin_8, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_DOWN },  // MISO
        { GPIO_Pin_6, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL }, // CS
      }, {GPIO_PinSource7, GPIO_PinSource9, GPIO_PinSource8, GPIO_PinSource6}, 
      {&RCC->APB2ENR, &RCC->AHB1ENR}, {RCC_APB2Periph_SPI5, RCC_AHB1Periph_GPIOF }, GPIO_AF_SPI5 },
    { SPI6, { GPIOG, GPIOG, GPIOG, GPIOG}, {   // SPI6
        { GPIO_Pin_13, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // SCK
        { GPIO_Pin_14, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL },  // MOSI
        { GPIO_Pin_12, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_OD, GPIO_PuPd_NOPULL },  // MISO
        { GPIO_Pin_8, GPIO_Mode_AF, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL }, // CS
      }, {GPIO_PinSource13, GPIO_PinSource14, GPIO_PinSource12, GPIO_PinSource8}, 
      {&RCC->APB2ENR, &RCC->AHB1ENR}, {RCC_APB2Periph_SPI6, RCC_AHB1Periph_GPIOG }, GPIO_AF_SPI6 },
};

void spi_init(uint8_t id)
{
    if (!((id >= 1) && (id <= 6)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;

    int i;
    for (i = 0; i < 2; i++)
        *(spiInitDef->bridge[i]) |= spiInitDef->peripheral[i];

    for (i = 0; i < 4; i++)
    {
       GPIO_Init(spiInitDef->tdef[i], &spiInitDef->gpiodef[i]);
       GPIO_PinAFConfig(spiInitDef->tdef[i], spiInitDef->pinsource[i], spiInitDef->af);
    }

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
    spidef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; 
    spidef.SPI_FirstBit = SPI_FirstBit_MSB; 
    spidef.SPI_CRCPolynomial = 7;

    SPI_Init(spiInitDef->id, &spidef); 
    SPI_Cmd(spiInitDef->id, ENABLE);
    SPI_SSOutputCmd(spiInitDef->id, DISABLE);

    spi_stop(id);
}

void spi_start(uint8_t id)
{
    if (!((id >= 1) && (id <= 6)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;
    SPI_NSSInternalSoftwareConfig(spiInitDef->id, SPI_NSSInternalSoft_Reset);
}

void spi_stop(uint8_t id)
{
    if (!((id >= 1) && (id <= 6)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;
    SPI_NSSInternalSoftwareConfig(spiInitDef->id, SPI_NSSInternalSoft_Set);
}

void spi_read(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    if (!((id >= 1) && (id <= 6)))
        return;

    struct spiInitDef_t * spiInitDef = spiInitDefs + id - 1;

    while(nb--)
    {
        taskENTER_CRITICAL();

        //send, because that's the rule
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(spiInitDef->id, 0xFF);

        //read data
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_RXNE) == RESET);
        *buffer++ = (uint8_t) SPI_I2S_ReceiveData(spiInitDef->id);

        //wait it is ready
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_BSY) == SET);

        taskEXIT_CRITICAL();
    }
}


void spi_write(uint8_t id, uint8_t *buffer, uint8_t nb)
{
    if (!((id >= 1) && (id <= 6)))
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

        //wait it is ready
        while (SPI_I2S_GetFlagStatus(spiInitDef->id, SPI_I2S_FLAG_BSY) == SET);

        taskEXIT_CRITICAL();
    }
}


void spi_get_register(uint8_t id, uint8_t address, uint8_t *buffer, uint8_t nb)
{
    //set the read bit
    address |= READ_BYTE;

    //reset multiple byte bit if necessary
//    if (nb <= 1)
//        address &= 0xBF;
    //set multiple byte bit if necessary
    if (nb > 1)
        address |= MULTIBYTE_BYTE;
    //send the request
    spi_write(id, &address, 1);

    //get the value
    spi_read(id, buffer, nb);
}

void spi_set_register(uint8_t id, uint8_t address, uint8_t *buffer, uint8_t nb)
{
    //reset the read bit
    address &= (0xff ^ READ_BYTE);

    //set multiple byte bit if necessary
    if (nb > 1)
        address |= MULTIBYTE_BYTE;
    //send the request
    spi_write(id, &address, 1);

    //send the value
    spi_write(id, buffer, nb);
}

