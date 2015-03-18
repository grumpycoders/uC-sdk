#include <gpio.h>

#define LED1_wire make_pin(GPIO_PORT_B, 18)
#define LED2_wire make_pin(GPIO_PORT_B, 20)
#define LED3_wire make_pin(GPIO_PORT_B, 21)
#define LED4_wire make_pin(GPIO_PORT_B, 23)

static void setupLEDs() {
    gpio_config(LED1_wire, pin_dir_write, pull_up);
    gpio_config(LED2_wire, pin_dir_write, pull_up);
    gpio_config(LED3_wire, pin_dir_write, pull_up);
    gpio_config(LED4_wire, pin_dir_write, pull_up);
}

static void litLED(int led, int value) {
    if ((led > 4) || (led < 1))
        return;

    pin_t ledpin;

    switch (led) {
        case 1: ledpin = LED1_wire; break;
        case 2: ledpin = LED2_wire; break;
        case 3: ledpin = LED3_wire; break;
        case 4: ledpin = LED4_wire; break;
    }

    gpio_set(ledpin, value);
}

int main() {
    setupLEDs();

    litLED(1, 1);
    litLED(3, 1);

    return 0;
}
