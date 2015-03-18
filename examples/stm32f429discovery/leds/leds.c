#include <gpio.h>

int main() {
    //Initialize the pin_t structure with the pin port and number
    //On this board there is a LED on PG13
    pin_t pin = make_pin(GPIO_PORT_G, 13);

    //configure the pin for output. 
    gpio_config(pin, pin_dir_write, pull_down);

    //set the pin to HIGH
    gpio_set(pin, 1);

    return 0;
}
