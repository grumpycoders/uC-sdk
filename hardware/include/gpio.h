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
static __inline__ pin_t make_pin(uint8_t port, uint8_t pin) { pin_t p = { port, pin }; return p; }

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

END_DECL
