#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <BoardConsole.h>
#include <stdio.h>
#include <fio.h>
#include <malloc_wrapper.h>
#include <semiio.h>
#include <lis3dsh.h>

static const pin_t led_w = MAKE_PIN(GPIO_PORT_D, 12);
static const pin_t led_n = MAKE_PIN(GPIO_PORT_D, 13);
static const pin_t led_e = MAKE_PIN(GPIO_PORT_D, 14);
static const pin_t led_s = MAKE_PIN(GPIO_PORT_D, 15);

int main() {
    init_malloc_wrapper();
#ifdef DEBUG_OUTPUT
    register_semiio();
#endif

    lis3dsh_t lis3dsh;

    lis3dsh.ssp_port = MAKE_SSP_PORT(ssp_port_1, MAKE_PIN(GPIO_PORT_A, 5), MAKE_PIN(GPIO_PORT_A, 7), MAKE_PIN(GPIO_PORT_A, 6));
    lis3dsh.cs = MAKE_PIN(GPIO_PORT_E, 3);
    lis3dsh_init(&lis3dsh);

    gpio_config(led_w, pin_dir_write, pull_up);
    gpio_config(led_n, pin_dir_write, pull_up);
    gpio_config(led_e, pin_dir_write, pull_up);
    gpio_config(led_s, pin_dir_write, pull_up);
    gpio_set(led_w, 0);
    gpio_set(led_n, 0);
    gpio_set(led_e, 0);
    gpio_set(led_s, 0);

    float axis[3];
    int n, e, s, w;

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
