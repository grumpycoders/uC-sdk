#include <FreeRTOS.h>
#include <task.h>
#include <BoardConsole.h>
#include <stdio.h>
#include <malloc_wrapper.h>

#include <lcd.h>

int main() {
    init_malloc_wrapper();

    lcd_t lcd;

    pin_t sclk = { .port = gpio_port_c, .pin = 10 };
    pin_t mosi = { .port = gpio_port_c, .pin = 12 };
    pin_t miso = { .port = gpio_port_c, .pin = 11 };
    pin_t cs   = { .port = gpio_port_c, .pin = 8 };
    pin_t rs   = { .port = gpio_port_f, .pin = 12 };
    pin_t rw   = { .port = gpio_port_f, .pin = 13 };
    pin_t e    = { .port = gpio_port_c, .pin = 6 };

    ssp_port_t ssp = { .ssp = ssp_3, .sclk = sclk, .mosi = mosi, .miso = miso};

    if (!lcd_init(&lcd, ssp, cs, rs, rw, e))
    {
        printf("Cannot initialize lcd");
        return 0;
    }

    lcd_move_to(&lcd, 0, 0);
    lcd_print_string(&lcd, "Test");
    while(1)
    {

    }

    return 0;
}
