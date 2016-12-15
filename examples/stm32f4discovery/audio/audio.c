#include <dac.h>
#include <adc.h>
#include <timer.h>
#include <gpio.h>

pin_t timestamp;

//should be correct if system_stm32f4xx.c is setup correctly
#define APB1FREQ 42000000

//sampling callback: run at 44kHz
void sample()
{
    gpio_set(timestamp, 1); //for oscilloscope debug

    //get sample from input
    uint16_t value = adc_get(adc_1);

    //set sample to output
    dac_set16(dac_1, value);

    gpio_set(timestamp, 0); //for oscilloscope debug
}

int main()
{
    //debug pin on PB3 to measure computing time in the callback
    timestamp = make_pin(gpio_port_b, 2);
    gpio_config(timestamp, pin_dir_write, pull_down);

    //configure DAC (1) for output on PA4
    dac_port_t dac = { .pin = { .port = gpio_port_a, .pin = 4}, .dac = dac_1 };
    dac_config(dac);

    //configure ADC (1 channel 3) for input on PA3
    adc_config_all();
    adc_config_single(adc_1, 3, make_pin(gpio_port_a, 3));

    //configure timer (2 channel 3) for 44kHz sampling
    timer_channel_t timer = { .timer = 2, .channel = 3 };
    timer_config(timer.timer, 1, APB1FREQ / 44000);
    timer_irq_init(timer, event_update, sample);

    while(1);

    return 1;
}
