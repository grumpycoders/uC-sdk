#include "lpc17xx_nvic.h"
#include "lpc17xx_clkpwr.h"

extern uintptr_t __cs3_interrupt_vector_mutable[];

void BoardEarlyInit() {
    NVIC_SetVTOR((uintptr_t) __cs3_interrupt_vector_mutable);
}

void BoardLateInit() {
}

void BoardShutdown() {
}

void BoardExceptionHandler(int code) {
    volatile int i;
    for (i = 0; i < 100000; i++);
    CLKPWR_DeepPowerDown();
}
