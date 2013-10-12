#include "stm32f10x_adc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_can.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_wwdg.h"

void cpu_deinit_all() {
    //ADC_DeInit(ADC_TypeDef* ADCx);
    BKP_DeInit();
    //CAN_DeInit(CAN_TypeDef* CANx);
    CEC_DeInit();
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
