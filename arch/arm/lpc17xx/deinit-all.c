#include "LPC17xx.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_can.h"
#include "lpc17xx_emac.h"
#include "lpc17xx_exti.h"
#include "lpc17xx_i2c.h"
#include "lpc17xx_i2s.h"
#include "lpc17xx_nvic.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_qei.h"
#include "lpc17xx_rit.h"
#include "lpc17xx_rtc.h"
#include "lpc17xx_spi.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_uart.h"

void cpu_deinit_all() {
//**    ADC_DeInit(LPC_ADC);
    CAN_DeInit(LPC_CAN1);
    CAN_DeInit(LPC_CAN2);
    EMAC_DeInit();
    EXTI_DeInit();
    I2C_DeInit(LPC_I2C0);
    I2C_DeInit(LPC_I2C1);
    I2C_DeInit(LPC_I2C2);
    I2S_DeInit(LPC_I2S);
    NVIC_DeInit();
    NVIC_SCBDeInit();
    PWM_DeInit(LPC_PWM1);
    QEI_DeInit(LPC_QEI);
//**    RIT_DeInit(LPC_RIT);
    RTC_DeInit(LPC_RTC);
    SPI_DeInit(LPC_SPI);
    SSP_DeInit(LPC_SSP0);
    SSP_DeInit(LPC_SSP1);
    UART_DeInit(LPC_UART0);
    UART_DeInit((LPC_UART_TypeDef *) LPC_UART1);
//**    UART_DeInit(LPC_UART2);
//**    UART_DeInit(LPC_UART3);
    TIM_DeInit(LPC_TIM0);
    TIM_DeInit(LPC_TIM1);
//**    TIM_DeInit(LPC_TIM2);
//**    TIM_DeInit(LPC_TIM3);
}
