#include <gpio.h>
#include <timer.h>

int main() {
    //Initialize the pin_t structure with the pin port and number
    //On the evaluation board there is a LED on PA1
    pin_t pin = make_pin(gpio_port_a, 1);

    //Initialize Timer 3, channel 3
    timer_port_t timer = { .timer = 2, .channel = 2 };

    timer_config(timer, 4096, 1000);

    timer_pwmchannel_init(timer, pin, 500);

    while(1){}

    return 0;
}
