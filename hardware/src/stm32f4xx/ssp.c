#include "ssp.h"

#include <stdlib.h>

#include "hardware.h"

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
    ssp_mode_t mode = ssp_port.mode;
    pin_t sclk = ssp_port.sclk;
    pin_t miso = ssp_port.miso;
    pin_t mosi = ssp_port.mosi;
    pin_t ss = ssp_port.ss;

    SPI_TypeDef * id = spis[ssp];

    switch (ssp) {
        case ssp_1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        break;
        case ssp_2:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        break;
        case ssp_3:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
        break;
        case ssp_4:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI4, ENABLE);
        break;
        case ssp_5:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5, ENABLE);
        break;
        case ssp_6:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);
        break;
        default:
        return;
    }

    uint8_t af;
    if (ssp == ssp_3)
        af = 6;
    else
        af = 5;


    if (mode == ssp_slave)
    {
        gpio_config_alternate(sclk, pin_dir_read, pull_down, af);
        gpio_config_alternate(miso, pin_dir_write, pull_down, af);
        gpio_config_alternate(mosi, pin_dir_read, pull_down, af);
    }
    else
    {
        gpio_config_alternate(sclk, pin_dir_write, pull_down, af);
        gpio_config_alternate(miso, pin_dir_read, pull_down, af);
        gpio_config_alternate(mosi, pin_dir_write, pull_down, af);
    }
    if (valid_pin(ss))
        gpio_config_alternate(ss, pin_dir_read, pull_up, af);

    SPI_InitTypeDef spi;

    SPI_I2S_DeInit(id);
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    if (valid_pin(ss))
        spi.SPI_NSS = SPI_NSS_Soft;
    else
        spi.SPI_NSS = SPI_NSS_Hard;

    spi.SPI_BaudRatePrescaler = compute_prescaler(clock);
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;
    if (mode == ssp_slave)
        spi.SPI_Mode = SPI_Mode_Slave;
    else
        spi.SPI_Mode = SPI_Mode_Master;
    SPI_Init(id, &spi);

    SPI_Cmd(id, ENABLE);
}

uint8_t ssp_readwrite(ssp_t ssp, uint8_t value) {
    SPI_TypeDef * id = spis[ssp];

    while (SPI_I2S_GetFlagStatus(id, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(id, value);
    while (SPI_I2S_GetFlagStatus(id, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(id);
}

#if defined(STM32F40_41xxx)
uint8_t irq_ssp_channels[] = { 0, SPI1_IRQn, SPI2_IRQn , SPI3_IRQn };
#elif defined(STM32F427_437xx) || defined(STM32F429_439xx)
uint8_t irq_ssp_channels[] = { 0, SPI1_IRQn, SPI2_IRQn , SPI3_IRQn, SPI4_IRQn, SPI5_IRQn, SPI6_IRQn };
#elif defined(STM32F446xx)
uint8_t irq_ssp_channels[] = { 0, SPI1_IRQn, SPI2_IRQn , SPI3_IRQn, SPI4_IRQn };
#endif
#define IRQ_SSP_MAX (6 * 2)
static void (*irq_ssp_callback[IRQ_SSP_MAX])();

__attribute__((constructor)) static void ssp_irq_callback_init() {
    int i;
    for (i = 0; i < IRQ_SSP_MAX; i++)
        irq_ssp_callback[i] = NULL;
}

static void set_ssp_callback(ssp_t ssp, irq_ssp_event_t event, void (*cb)()) {
    irq_ssp_callback[ssp * 2 + event] = cb;
}

static void call_ssp_callback(ssp_t ssp, irq_ssp_event_t event) {
    if (irq_ssp_callback[ssp * 2 + event]) {
        irq_ssp_callback[ssp * 2 + event]();
    }
}

void SPI1_IRQHandler(void){
    if (SPI_I2S_GetITStatus(spis[1], SPI_I2S_IT_RXNE) == SET)
    {
        call_ssp_callback(ssp_1, event_read);
        SPI_I2S_ClearITPendingBit(spis[1], SPI_I2S_IT_RXNE);
    }
    if (SPI_I2S_GetITStatus(spis[1], SPI_I2S_IT_TXE) == SET)
    {
        call_ssp_callback(ssp_1, event_write);
        SPI_I2S_ClearITPendingBit(spis[1], SPI_I2S_IT_TXE);
    }
}

void SPI2_IRQHandler(void){
    if (SPI_I2S_GetITStatus(spis[2], SPI_I2S_IT_RXNE) == SET)
    {
        call_ssp_callback(ssp_2, event_read);
        SPI_I2S_ClearITPendingBit(spis[2], SPI_I2S_IT_RXNE);
    }
    if (SPI_I2S_GetITStatus(spis[2], SPI_I2S_IT_TXE) == SET)
    {
        call_ssp_callback(ssp_2, event_write);
        SPI_I2S_ClearITPendingBit(spis[2], SPI_I2S_IT_TXE);
    }
}

void SPI3_IRQHandler(void){
    if (SPI_I2S_GetITStatus(spis[3], SPI_I2S_IT_RXNE) == SET)
    {
        call_ssp_callback(ssp_3, event_read);
        SPI_I2S_ClearITPendingBit(spis[3], SPI_I2S_IT_RXNE);
    }
    if (SPI_I2S_GetITStatus(spis[3], SPI_I2S_IT_TXE) == SET)
    {
        call_ssp_callback(ssp_3, event_write);
        SPI_I2S_ClearITPendingBit(spis[3], SPI_I2S_IT_TXE);
    }
}

void SPI4_IRQHandler(void){
    if (SPI_I2S_GetITStatus(spis[4], SPI_I2S_IT_RXNE) == SET)
    {
        call_ssp_callback(ssp_4, event_read);
        SPI_I2S_ClearITPendingBit(spis[4], SPI_I2S_IT_RXNE);
    }
    if (SPI_I2S_GetITStatus(spis[4], SPI_I2S_IT_TXE) == SET)
    {
        call_ssp_callback(ssp_4, event_write);
        SPI_I2S_ClearITPendingBit(spis[4], SPI_I2S_IT_TXE);
    }
}

void SPI5_IRQHandler(void){
    if (SPI_I2S_GetITStatus(spis[5], SPI_I2S_IT_RXNE) == SET)
    {
        call_ssp_callback(ssp_5, event_read);
        SPI_I2S_ClearITPendingBit(spis[5], SPI_I2S_IT_RXNE);
    }
    if (SPI_I2S_GetITStatus(spis[5], SPI_I2S_IT_TXE) == SET)
    {
        call_ssp_callback(ssp_5, event_write);
        SPI_I2S_ClearITPendingBit(spis[5], SPI_I2S_IT_TXE);
    }
}

void SPI6_IRQHandler(void){
    if (SPI_I2S_GetITStatus(spis[6], SPI_I2S_IT_RXNE) == SET)
    {
        call_ssp_callback(ssp_6, event_read);
        SPI_I2S_ClearITPendingBit(spis[6], SPI_I2S_IT_RXNE);
    }
    if (SPI_I2S_GetITStatus(spis[6], SPI_I2S_IT_TXE) == SET)
    {
        call_ssp_callback(ssp_6, event_write);
        SPI_I2S_ClearITPendingBit(spis[6], SPI_I2S_IT_TXE);
    }
}

void ssp_slave_start_read(ssp_t ssp){
    if (ssp < ssp_1 || ssp > ssp_6)
        return;
    SPI_I2S_ITConfig(spis[ssp], SPI_I2S_IT_RXNE, ENABLE);
}

void ssp_slave_stop_read(ssp_t ssp){
    if (ssp < ssp_1 || ssp > ssp_6)
        return;
    SPI_I2S_ITConfig(spis[ssp], SPI_I2S_IT_RXNE, DISABLE);
}

void ssp_slave_start_write(ssp_t ssp){
    if (ssp < ssp_1 || ssp > ssp_6)
        return;
    SPI_I2S_ITConfig(spis[ssp], SPI_I2S_IT_TXE, ENABLE);
}

void ssp_slave_stop_write(ssp_t ssp){
    if (ssp < ssp_1 || ssp > ssp_6)
        return;
    SPI_I2S_ITConfig(spis[ssp], SPI_I2S_IT_TXE, DISABLE);
}

void ssp_irq_init(ssp_t ssp, irq_ssp_event_t event, void (*cb)()){
    NVIC_InitTypeDef nvic;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    nvic.NVIC_IRQChannel = irq_ssp_channels[ssp];
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    SPI_I2S_ClearITPendingBit(spis[ssp], SPI_I2S_IT_RXNE);
    SPI_I2S_ClearITPendingBit(spis[ssp], SPI_I2S_IT_TXE);

    set_ssp_callback(ssp, event, cb);
}
