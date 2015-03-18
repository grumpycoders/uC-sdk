#include <FreeRTOS.h>
#include <task.h>
#include <BoardConsole.h>
#include <stdio.h>
//#include <fio.h>
#include <malloc_wrapper.h>
#include <semiio.h>
#include <l3gd20.h>

#define led_w make_pin(GPIO_PORT_G, 13)
#define led_e make_pin(GPIO_PORT_G, 14)

int main() {
    init_malloc_wrapper();
#ifdef DEBUG_OUTPUT
    register_semiio();
#endif
    gpio_config(led_w, pin_dir_write, pull_up);
    gpio_config(led_e, pin_dir_write, pull_up);
    gpio_set(led_w, 0);
    gpio_set(led_e, 0);

    l3gd20_t l3gd20;

    pin_t sclk = { .port = GPIO_PORT_F, .pin = 7 };
    pin_t mosi = { .port = GPIO_PORT_F, .pin = 9 };
    pin_t miso = { .port = GPIO_PORT_F, .pin = 8 };
    pin_t cs   = { .port = GPIO_PORT_C, .pin = 1 };

    ssp_port_t ssp = { .ssp = ssp_port_5, .sclk = sclk, .mosi = mosi, .miso = miso };

    if (!l3gd20_init_ssp(&l3gd20, ssp, cs))
    {
        printf("Cannot initialize gyroscope");
        return 0;
    }

    float axis[3];
    int e = 0, w = 0;
    while (1) {
        l3gd20_read(&l3gd20, axis);
#ifdef DEBUG_OUTPUT
        char output[128];
        sprintf(output, "x = %5.2f, y = %5.2f, z = %5.2f\n", axis[0], axis[1], axis[2]);
        printf("%s", output);
#endif

        if (axis[1] >= 3000.0f) {
            e = 1;
            w = 0;
        } else if (axis[1] <= -3000.0f) {
            e = 0;
            w = 1;
        } else if (axis[1] >= -2000.0f && axis[1] <= 2000.0f) {
            e = 0;
            w = 0;
        }

        gpio_set(led_e, e);
        gpio_set(led_w, w);
    }

    return 0;
}
