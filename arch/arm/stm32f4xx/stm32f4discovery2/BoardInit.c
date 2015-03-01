#include "stm32f4xx_rcc.h"

void BoardEarlyInit() {
	/* Select HSE as system clock source */
/*    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSE;
    SystemCoreClockUpdate();*/
}

void BoardInit() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

void BoardLateInit() {
}

void BoardShutdown() {
}

void BoardExceptionHandler(int code) {
}
