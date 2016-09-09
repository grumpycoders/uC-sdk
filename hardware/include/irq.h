#pragma once

#include <decl.h>
#include <stdint.h>

#include "gpio.h"
#include "timer.h"

BEGIN_DECL

typedef enum
{
    rising = 0,
    falling = 1,
    change = 2,
} irq_trigger_t;

void set_timer(uint32_t ms, void (*cb) ());
void unset_timer();

void irq_gpio_init(pin_t pin, void (*cb)(), irq_trigger_t tt);
void irq_gpio_deinit(pin_t pin);

END_DECL
