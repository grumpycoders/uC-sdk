#include "lpc17xx_nvic.h"

extern void * __cs3_interrupt_vector_mutable;

void BoardEarlyInit() {
    //NVIC_SetVTOR((uint32_t) __cs3_interrupt_vector_mutable);
}

void BoardLateInit() {
}

void BoardShutdown() {
}

void BoardExceptionHandler(int code) {
}
