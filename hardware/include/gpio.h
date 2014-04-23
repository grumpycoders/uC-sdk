#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

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

END_DECL
