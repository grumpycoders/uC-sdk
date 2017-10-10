#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL
#pragma once

void *set_irq_handler(uint16_t irq, void * handler);

void set_timer(uint32_t ms, void (*cb) ());
void unset_timer();

END_DECL
