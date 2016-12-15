#include <timer.h>

static uint8_t toggle = 0;

void update() {
    toggle ^= 1;
}

int main() {
    //Initialize the pin_t structure with the pin port and number
    //On this board there is a LED on PD12
    pin_t pin = make_pin(gpio_port_d, 12);

    //configure the pin for output.
    gpio_config(pin, pin_dir_write, pull_down);

    //Initialize Timer 1, channel 1
    timer_channel_t timer = { .timer = timer_2, .channel = 3 };

    //180000000/(18000 * 10000) = 1Hz
    timer_config(timer_2, timer_get_clock_freq(timer_2)/10000, 1000);

    //Call the update callback at each timer  update event
    timer_irq_init(timer, event_update, update);

    while(1)
        gpio_set(pin, toggle);

    return 0;
}
