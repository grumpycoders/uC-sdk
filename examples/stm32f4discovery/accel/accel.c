#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <BoardConsole.h>
#include <stdio.h>
#include <fio.h>
#include <malloc_wrapper.h>
#include <semiio.h>
#include <lis3dsh.h>

#define led_w make_pin(GPIO_PORT_D, 12)
#define led_n make_pin(GPIO_PORT_D, 13)
#define led_e make_pin(GPIO_PORT_D, 14)
#define led_s make_pin(GPIO_PORT_D, 15)

int main() {
    init_malloc_wrapper();
#ifdef DEBUG_OUTPUT
    register_semiio();
#endif

    lis3dsh_t lis3dsh;

    pin_t sclk = { .port = GPIO_PORT_A, .pin = 5 };
    pin_t mosi = { .port = GPIO_PORT_A, .pin = 7 };
    pin_t miso = { .port = GPIO_PORT_A, .pin = 6 };
    pin_t cs   = { .port = GPIO_PORT_E, .pin = 3 };

    ssp_port_t ssp = { .ssp = ssp_port_1, .sclk = sclk, .mosi = mosi, .miso = miso };

    lis3dsh_init_ssp(&lis3dsh, ssp, cs);

    gpio_config(led_w, pin_dir_write, pull_up);
    gpio_config(led_n, pin_dir_write, pull_up);
    gpio_config(led_e, pin_dir_write, pull_up);
    gpio_config(led_s, pin_dir_write, pull_up);
    gpio_set(led_w, 0);
    gpio_set(led_n, 0);
    gpio_set(led_e, 0);
    gpio_set(led_s, 0);

    float axis[3];
    int n = 0, e = 0, s = 0, w = 0;

    while (1) {
        lis3dsh_read(&lis3dsh, axis);
#ifdef DEBUG_OUTPUT
        char output[128];
        sprintf(output, "x = %5.2f, y = %5.2f, z = %5.2f\n", axis[0], axis[1], axis[2]);
        printf("%s", output);
#endif

        if (axis[0] >= 600.0f) {
            e = 1;
            w = 0;
        } else if (axis[0] <= -600.0f) {
            e = 0;
            w = 1;
        } else if (axis[0] >= -300.0f && axis[0] <= 300.0f) {
            e = 0;
            w = 0;
        }

        if (axis[1] >= 600.0f) {
            n = 1;
            s = 0;
        } else if (axis[1] <= -600.0f) {
            n = 0;
            s = 1;
        } else if (axis[1] >= -300.0f && axis[1] <= 300.0f) {
            n = 0;
            s = 0;
        }

        gpio_set(led_n, n);
        gpio_set(led_e, e);
        gpio_set(led_s, s);
        gpio_set(led_w, w);
    }

    return 0;
}
