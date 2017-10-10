#pragma once

#include <stm32f4xx.h>

extern GPIO_TypeDef * const stm32f4xx_gpio_ports[];


typedef enum {
    CS3STACK_IRQ_handler,
    CS3RESET_IRQ_handler,
    NMI_IRQ_handler,
    HardFault_IRQ_handler,
    MemManage_IRQ_handler,
    BusFault_IRQ_handler,
    UsageFault_IRQ_handler,
    RESERVED1_IRQ_handler,
    RESERVED2_IRQ_handler,
    RESERVED3_IRQ_handler,
    RESERVED4_IRQ_handler,
    SVC_IRQ_handler,
    DebugMon_IRQ_handler,
    RESERVED5_IRQ_handler,
    PendSV_IRQ_handler,
    SysTick_IRQ_handler,
    WDT_IRQ_handler,
    PVD_IRQ_handler,
    TAMP_STAMP_IRQ_handler,
    RTC_WKUP_IRQ_handler,
    FLASH_IRQ_handler,
    RCC_IRQ_handler,
    EXTI0_IRQ_handler,
    EXTI1_IRQ_handler,
    EXTI2_IRQ_handler,
    EXTI3_IRQ_handler,
    EXTI4_IRQ_handler,
    DMA1_Stream0_IRQ_handler,
    DMA1_Stream1_IRQ_handler,
    DMA1_Stream2_IRQ_handler,
    DMA1_Stream3_IRQ_handler,
    DMA1_Stream4_IRQ_handler,
    DMA1_Stream5_IRQ_handler,
    DMA1_Stream6_IRQ_handler,
    ADC_IRQ_handler,
    CAN1_TX_IRQ_handler,
    CAN1_RX0_IRQ_handler,
    CAN1_RX1_IRQ_handler,
    CAN1_SCE_IRQ_handler,
    EXTI9_5_IRQ_handler,
    TIM1_BRK_TIM9_IRQ_handler,
    TIM1_UP_TIM10_IRQ_handler,
    TIM1_TRG_COM_TIM11_IRQ_handler,
    TIM1_CC_IRQ_handler,
    TIM2_IRQ_handler,
    TIM3_IRQ_handler,
    TIM4_IRQ_handler,
    I2C1_EV_IRQ_handler,
    I2C1_ER_IRQ_handler,
    I2C2_EV_IRQ_handler,
    I2C2_ER_IRQ_handler,
    SPI1_IRQ_handler,
    SPI2_IRQ_handler,
    USART1_IRQ_handler,
    USART2_IRQ_handler,
    USART3_IRQ_handler,
    EXTI15_10_IRQ_handler,
    RTC_Alarm_IRQ_handler,
    OTG_FS_WKUP_IRQ_handler,
    TIM8_BRK_TIM12_IRQ_handler,
    TIM8_UP_TIM13_IRQ_handler,
    TIM8_TRG_COM_TIM14_IRQ_handler,
    TIM8_CC_IRQ_handler,
    DMA1_Stream7_IRQ_handler,
    FSMC_IRQ_handler,
    SDIO_IRQ_handler,
    TIM5_IRQ_handler,
    SPI3_IRQ_handler,
    UART4_IRQ_handler,
    UART5_IRQ_handler,
    TIM6_DAC_IRQ_handler,
    TIM7_IRQ_handler,
    DMA2_Stream0_IRQ_handler,
    DMA2_Stream1_IRQ_handler,
    DMA2_Stream2_IRQ_handler,
    DMA2_Stream3_IRQ_handler,
    DMA2_Stream4_IRQ_handler,
    ETH_IRQ_handler,
    ETH_WKUP_IRQ_handler,
    CAN2_TX_IRQ_handler,
    CAN2_RX0_IRQ_handler,
    CAN2_RX1_IRQ_handler,
    CAN2_SCE_IRQ_handler,
    OTG_FS_IRQ_handler,
    DMA2_Stream5_IRQ_handler,
    DMA2_Stream6_IRQ_handler,
    DMA2_Stream7_IRQ_handler,
    USART6_IRQ_handler,
    I2C3_EV_IRQ_handler,
    I2C3_ER_IRQ_handler,
    OTG_HS_EP1_OUT_IRQ_handler,
    OTG_HS_EP1_IN_IRQ_handler,
    OTG_HS_WKUP_IRQ_handler,
    OTG_HS_IRQ_handler,
    DCMI_IRQ_handler,
    CRYP_IRQ_handler,
    HASH_RNG_IRQ_handler,
    FPU_IRQ_handler,
    UART7_IRQ_handler,
    UART8_IRQ_handler,
    SPI4_IRQ_handler,
    SPI5_IRQ_handler,
    SPI6_IRQ_handler,
    SAI1_IRQ_handler,
    LCD_TFT_IRQ_handler,
    LCD_TFT_ERROR_IRQ_handler,
    DMA2D_IRQ_handler,
} IRQ_handler;
