#include "stm32f4xx_adc.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_cryp.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dbgmcu.h"
#include "stm32f4xx_dcmi.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_fsmc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_hash.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_iwdg.h"
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
    //ADC_DeInit(ADC_TypeDef* ADCx);
    //BKP_DeInit();
    //CAN_DeInit(CAN_TypeDef* CANx);
    //CEC_DeInit();
    DAC_DeInit();
    //DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx);
    EXTI_DeInit();
    //GPIO_DeInit(GPIO_TypeDef* GPIOx);
    //I2C_DeInit(I2C_TypeDef* I2Cx);
    PWR_DeInit();
    RCC_DeInit();
    SDIO_DeInit();
    //SPI_I2S_DeInit(SPI_TypeDef* SPIx);
    //TIM_DeInit(TIM_TypeDef* TIMx);
    //USART_DeInit(USART_TypeDef* USARTx);
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
