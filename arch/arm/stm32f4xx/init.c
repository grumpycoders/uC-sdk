#include "stm32f4xx_adc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_cryp.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dbgmcu.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_dma2d.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_hash.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_ltdc.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_wwdg.h"
#include "misc.h"

void cpu_deinit_all() {
    ADC_DeInit();
    CAN_DeInit(CAN1);
    CAN_DeInit(CAN2);
    CRYP_DeInit();
    DAC_DeInit();
    DCMI_DeInit();
    DMA_DeInit(DMA1_Stream0);
    DMA_DeInit(DMA1_Stream1);
    DMA_DeInit(DMA1_Stream2);
    DMA_DeInit(DMA1_Stream3);
    DMA_DeInit(DMA1_Stream4);
    DMA_DeInit(DMA1_Stream5);
    DMA_DeInit(DMA1_Stream6);
    DMA_DeInit(DMA1_Stream7);
    DMA_DeInit(DMA2_Stream0);
    DMA_DeInit(DMA2_Stream1);
    DMA_DeInit(DMA2_Stream2);
    DMA_DeInit(DMA2_Stream3);
    DMA_DeInit(DMA2_Stream4);
    DMA_DeInit(DMA2_Stream5);
    DMA_DeInit(DMA2_Stream6);
    DMA_DeInit(DMA2_Stream7);
    DMA2D_DeInit();
    EXTI_DeInit();
    GPIO_DeInit(GPIOA);
    GPIO_DeInit(GPIOB);
    GPIO_DeInit(GPIOC);
    GPIO_DeInit(GPIOD);
    GPIO_DeInit(GPIOE);
    GPIO_DeInit(GPIOF);
    GPIO_DeInit(GPIOG);
    GPIO_DeInit(GPIOH);
    GPIO_DeInit(GPIOI);
    GPIO_DeInit(GPIOJ);
    GPIO_DeInit(GPIOK);
    HASH_DeInit();
    I2C_DeInit(I2C1);
    I2C_DeInit(I2C2);
    I2C_DeInit(I2C3);
    LTDC_DeInit();
    PWR_DeInit();
    RCC_DeInit();
    RNG_DeInit();
    RTC_DeInit();
    //SAI_DeInit(SAI_TypeDef* SAIx);
    SDIO_DeInit();
    SPI_I2S_DeInit(SPI1);
    SPI_I2S_DeInit(SPI2);
    SPI_I2S_DeInit(SPI3);
    SPI_I2S_DeInit(SPI4);
    SPI_I2S_DeInit(SPI5);
    SPI_I2S_DeInit(SPI6);
    SPI_I2S_DeInit(I2S2ext);
    SPI_I2S_DeInit(I2S3ext);
    SYSCFG_DeInit();
    TIM_DeInit(TIM1);
    TIM_DeInit(TIM2);
    TIM_DeInit(TIM3);
    TIM_DeInit(TIM4);
    TIM_DeInit(TIM5);
    TIM_DeInit(TIM6);
    TIM_DeInit(TIM7);
    TIM_DeInit(TIM8);
    TIM_DeInit(TIM9);
    TIM_DeInit(TIM10);
    TIM_DeInit(TIM11);
    TIM_DeInit(TIM12);
    TIM_DeInit(TIM13);
    TIM_DeInit(TIM14);
    USART_DeInit(USART1);
    USART_DeInit(USART2);
    USART_DeInit(USART3);
    USART_DeInit(UART4);
    USART_DeInit(UART5);
    USART_DeInit(USART6);
    USART_DeInit(UART7);
    USART_DeInit(UART8);
    WWDG_DeInit();
}

extern uintptr_t __cs3_interrupt_vector_mutable[];

void cpu_early_init() {
    cpu_deinit_all();
    SystemInit();
}

void cpu_init() {
    NVIC_SetVectorTable(NVIC_VectTab_RAM, ((uintptr_t) __cs3_interrupt_vector_mutable) - NVIC_VectTab_RAM);
}

void cpu_late_init() {
}

__attribute__((section(".bootptr"))) uint32_t __bootptr = 0xf1e0f85f;
__attribute__((section(".som"))) uint32_t __som = 0xffffffff;
