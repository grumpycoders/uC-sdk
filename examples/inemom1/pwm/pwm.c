#include <gpio.h>
#include <timer.h>

int main() {
    //Initialize the pin_t structure with the pin port and number
    //On the evaluation board there is a LED on PA1
    pin_t pin = make_pin(GPIO_PORT_A, 1);

    //configure the timer. The PA1 pin uses Timer 2 Channel 2
    timer_init(2, 2, 4096, 1000);
    timer_init_pwmchannel(2, 2, pin, 500);

    while(1){}

    return 0;
}
