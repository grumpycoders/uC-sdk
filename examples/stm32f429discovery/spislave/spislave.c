#include <gpio.h>
#include <ssp.h>

#include <stdio.h>

pin_t led1, led2;

uint8_t rled = 0, tled = 0;

void slavereceived(){
    volatile uint8_t r = ssp_read(ssp_4);
    printf("received %x\n", r);
    //toggle the receive LED
    rled ^= 1;
    gpio_set(led2, rled);

    ssp_slave_stop_read(ssp_4);
    //slave write
    ssp_slave_start_write(ssp_4);
}

void slavesent(){
    ssp_write(ssp_4, 0x28);
    //toggle the transmit LED
    tled ^= 1;
    gpio_set(led1, tled);

    ssp_slave_stop_write(ssp_4);
    //slave read
    ssp_slave_start_read(ssp_4);
}

int main(){
    //LEDs on PG13 and PG14
    led1 = make_pin(gpio_port_g, 13);
    led2 = make_pin(gpio_port_g, 14);
    gpio_config(led1, pin_dir_write, pull_down);
    gpio_config(led2, pin_dir_write, pull_down);

    //SPI4
    pin_t sck = make_pin(gpio_port_e, 2);
    pin_t miso = make_pin(gpio_port_e, 5);
    pin_t mosi = make_pin(gpio_port_e, 6);
    ssp_port_t slave = { .sclk = sck, .mosi = mosi, .miso = miso, .ss = PIN_NULL, .mode = ssp_slave, .ssp = ssp_4 };
    ssp_config(slave, 8000000);

    //SSP slave callbacks
    ssp_irq_init(ssp_4, event_read, slavereceived);
    ssp_irq_init(ssp_4, event_write, slavesent);

    //slave read
    ssp_slave_start_read(ssp_4);

    while(1);

    return 0;
}