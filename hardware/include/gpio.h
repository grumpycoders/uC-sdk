#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

#define GPIO_PORT_A 0
#define GPIO_PORT_B 1
#define GPIO_PORT_C 2
#define GPIO_PORT_D 3
#define GPIO_PORT_E 4
#define GPIO_PORT_F 5
#define GPIO_PORT_G 6
#define GPIO_PORT_H 7
#define GPIO_PORT_I 8
#define GPIO_PORT_J 9
#define GPIO_PORT_K 10

// should be for CM3 - that's for everyone for now
typedef uint16_t pin_t;
#define MAKE_PIN(port, pin) ((pin_t)(((port & 0xff) << 8) | (pin & 0xff)))

typedef enum {
    pin_dir_read = 0,
    pin_dir_write = 1,
} pin_dir_t;

void gpio_config(pin_t pin, pin_dir_t dir);
void gpio_set(pin_t pin, int enabled);
int gpio_get(pin_t pin);

inline uint8_t get_port(pin_t pin);
inline uint8_t get_pin(pin_t pin);

END_DECL
