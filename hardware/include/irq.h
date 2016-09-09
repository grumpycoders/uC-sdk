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

END_DECL
