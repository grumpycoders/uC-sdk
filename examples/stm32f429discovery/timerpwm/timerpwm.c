#include <timer.h>

#include <stm32f4xx_tim.h>

int main() {
    //Initialize the pin_t structure with the pin port and number
    //Pin PB0 is connected to Timer 3 channel 3.
    //Plug an LED on this pin
    pin_t pin = make_pin(GPIO_PORT_B, 0);

    //Initialize Timer 3, channel 3
    timer_port_t timer = { .timer = 3, .channel = 3 };

    //180000000/(18000 * 10000) = 1Hz
    timer_config(timer, 18000, 10000);

    //Creates a PWM signal on the pin, duty cycle 8/10
    timer_pwmchannel_init(timer, pin, 8000);

    while(1){}

    return 0;
}
