#include <FreeRTOS.h>
#include <task.h>
#include <debug_frmwrk.h>
#include <lpc17xx_gpio.h>

#define LED1_wire 18
#define LED2_wire 20
#define LED3_wire 21
#define LED4_wire 23

static void setupLEDs() {
    GPIO_SetDir(1, (1 << LED1_wire) | (1 << LED2_wire) | (1 << LED3_wire) | (1 << LED4_wire), 1);
}

void litLED(int led, int value) {
    if ((led >= 4) || (led < 0))
        return;
    
    switch (led) {
        case 1: led = LED1_wire; break;
        case 2: led = LED2_wire; break;
        case 3: led = LED3_wire; break;
        case 4: led = LED4_wire; break;
    }
    
    if (value) {
        GPIO_SetValue(1, led);
    } else {
        GPIO_ClearValue(1, led);
    }
}

int main() {
    debug_frmwrk_init();
    setupLEDs();
    litLED(1, 1);
    litLED(2, 0);
    litLED(3, 1);
    litLED(4, 0);
    vTaskStartScheduler();
    return 0;
}
