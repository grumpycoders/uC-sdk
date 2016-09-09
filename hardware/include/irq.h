#pragma once

#include <decl.h>
#include <stdint.h>

#include "gpio.h"
#include "timer.h"

BEGIN_DECL

void set_timer(uint32_t ms, void (*cb) ());
void unset_timer();

END_DECL
