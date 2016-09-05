#include <gpio.h>
#include <irq.h>
#include <stdio.h>

static volatile int status = 0;

void toggleled(){
    status ^= 1;
}

int main() {
    //declare the pin structures for the led and the button
    pin_t led, button;

    //Initialize the pin_t structure with the pin port and number
    //On this board there is a button on PA0
    button = make_pin(GPIO_PORT_A, 0);

    //configure the pin for input.
    gpio_config(button, pin_dir_read, pull_down);

    //attach the callback to the rising trigger on this pin
    irq_gpio_init(button, toggleled, rising);

    //Initialize the pin_t structure with the pin port and number
    //On this board there is a LED on PG13
    led = make_pin(GPIO_PORT_G, 13);

    //Configure the pin for output.
    gpio_config(led, pin_dir_write, pull_down);

    //Loop
    while (1){
        gpio_set(led, status);
    }

    return 0;
}
