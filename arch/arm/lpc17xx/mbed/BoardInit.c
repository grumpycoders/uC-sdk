#include "lpc17xx_nvic.h"

extern uintptr_t __cs3_interrupt_vector_mutable[];

void BoardEarlyInit() {
    NVIC_SetVTOR((uintptr_t) __cs3_interrupt_vector_mutable);
}

void BoardLateInit() {
}

void BoardShutdown() {
}

void BoardExceptionHandler(int code) {
}
