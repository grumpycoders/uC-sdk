#include "lpc17xx_gpio.h"
#include "lpc17xx_clkpwr.h"

#define LED1_wire 18
#define LED2_wire 20
#define LED3_wire 21
#define LED4_wire 23

static void setupLEDs() {
    GPIO_SetDir(1, (1 << LED1_wire) | (1 << LED2_wire) | (1 << LED3_wire) | (1 << LED4_wire), 1);
}

static void litLED(int led, int value) {
    if ((led > 4) || (led < 0))
        return;
    
    switch (led) {
        case 1: led = 1 << LED1_wire; break;
        case 2: led = 1 << LED2_wire; break;
        case 3: led = 1 << LED3_wire; break;
        case 4: led = 1 << LED4_wire; break;
    }
    
    if (value) {
        GPIO_SetValue(1, led);
    } else {
        GPIO_ClearValue(1, led);
    }
}



extern uintptr_t __cs3_interrupt_vector_mutable[];

void BoardEarlyInit() {
    setupLEDs();
    litLED(1, 1);
}

void BoardInit() {
    litLED(2, 1);
}

void BoardLateInit() {
    litLED(3, 1);
}

void BoardShutdown() {
}

void BoardExceptionHandler(int code) {
    volatile int i;
    for (i = 0; i < 100000; i++);
    CLKPWR_DeepPowerDown();
}
