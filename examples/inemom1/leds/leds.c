#include <gpio.h>
#include <stdio.h>

int main() {
    //Initialize the pin_t structure with the pin port and number
    //On the evaluation board there is a LED on PA1
    pin_t pin = make_pin(GPIO_PORT_A, 1);

    //configure the pin for output.
    gpio_config(pin, pin_dir_write, pull_down);

    //set the pin to HIGH
    gpio_set(pin, 1);

    return 0;
}
