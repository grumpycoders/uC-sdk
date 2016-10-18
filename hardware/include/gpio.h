#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

typedef enum {
    gpio_port_a,
    gpio_port_b,
    gpio_port_c,
    gpio_port_d,
    gpio_port_e,
    gpio_port_f,
    gpio_port_g,
    gpio_port_h,
    gpio_port_i,
    gpio_port_j,
} gpio_port_t;

typedef enum
{
    rising = 0,
    falling = 1,
    change = 2,
} irq_trigger_t;

typedef struct {
    uint8_t port;
    uint8_t pin;
} pin_t;

_Static_assert(sizeof(pin_t) == 2, "pin_t isn't 16 bits-wide");

typedef enum {
    pin_dir_read = 0,
    pin_dir_write = 1,
} pin_dir_t;

typedef enum {
    pull_none = 0,
    pull_up = 1,
    pull_down = 2,
} pull_t;

//initialize a pin structure for use in functions below
static __inline__ pin_t make_pin(gpio_port_t port, uint8_t pin) { pin_t p = { port, pin }; return p; }

/*
    Initializes a GPIO
*/
void gpio_config(pin_t pin, pin_dir_t dir, pull_t pull);

/*
    Initializes a GPIO for an alternate function
*/
void gpio_config_alternate(pin_t pin, pin_dir_t dir, pull_t pull, uint8_t af);

/*
    Writes a value on a pin
*/
void gpio_set(pin_t pin, int enabled);

/*
    Reads a value on a pin
*/
uint8_t gpio_get(pin_t pin);

/*
    Use a callback on events from a GPIO pin
*/
void gpio_irq_init(pin_t pin, void (*cb)(), irq_trigger_t tt);

/*
    Disables a callback
*/
void gpio_irq_deinit(pin_t pin);


END_DECL
