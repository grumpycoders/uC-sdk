#include <gpio.h>
#include <ssp.h>

#include <stdio.h>

pin_t led1, led2;

uint8_t rled = 0, tled = 0;

void buttonpress(){
    //master write
    ssp_write(ssp_4, 0x42);
    //toggle the transmit LED
    tled ^= 1;
    gpio_set(led1, tled);

    //master read
    volatile uint8_t r = ssp_read(ssp_4);
    printf("received %x\n", r);
    //toggle the receive LED
    rled ^= 1;
    gpio_set(led2, rled);
}

int main(){
    //LEDs on PG13 and PG14
    led1 = make_pin(gpio_port_g, 13);
    led2 = make_pin(gpio_port_g, 14);
    gpio_config(led1, pin_dir_write, pull_down);
    gpio_config(led2, pin_dir_write, pull_down);

    //SPI4
    pin_t sclk = make_pin(gpio_port_e, 2);
    pin_t miso = make_pin(gpio_port_e, 5);
    pin_t mosi = make_pin(gpio_port_e, 6);

    ssp_port_t master = { 
        .ssp = ssp_4,
        .sclk = sclk,
        .mosi = mosi,
        .miso = miso,
        .ss = PIN_NULL,
        .mode = ssp_master,
        .polarity = ssp_polarity_mode_0
    };
    ssp_config(master, 8000000);

    //user button on PA0
    pin_t button = make_pin(gpio_port_a, 0);
    gpio_config(button, pin_dir_read, pull_down);
    gpio_irq_init(button, buttonpress, rising);

    while(1);

    return 0;
}