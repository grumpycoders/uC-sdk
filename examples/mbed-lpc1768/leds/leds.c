#include <gpio.h>

#define LED1_wire MAKE_PIN(1, 18)
#define LED2_wire MAKE_PIN(1, 20)
#define LED3_wire MAKE_PIN(1, 21)
#define LED4_wire MAKE_PIN(1, 23)

static void setupLEDs() {
    gpio_config(LED1_wire, pin_dir_write, pull_up);
    gpio_config(LED2_wire, pin_dir_write, pull_up);
    gpio_config(LED3_wire, pin_dir_write, pull_up);
    gpio_config(LED4_wire, pin_dir_write, pull_up);
}

static void litLED(int led, int value) {
    if ((led > 4) || (led < 1))
        return;

    switch (led) {
        case 1: led = LED1_wire; break;
        case 2: led = LED2_wire; break;
        case 3: led = LED3_wire; break;
        case 4: led = LED4_wire; break;
    }

    gpio_set(led, value);
}

int main() {
    setupLEDs();

    litLED(1, 1);
    litLED(3, 1);

    return 0;
}
