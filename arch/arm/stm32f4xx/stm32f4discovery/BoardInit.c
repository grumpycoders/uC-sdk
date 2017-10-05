#include "stm32f4xx_rcc.h"

void BoardEarlyInit() {
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
