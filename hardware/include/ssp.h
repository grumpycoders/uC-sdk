#pragma once

#include <decl.h>
#include <stdint.h>
#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    ssp_port_0,
    ssp_port_1,
} ssp_t;

BEGIN_DECL

void ssp_config(ssp_t ssp, uint32_t clock);
uint8_t ssp_readwrite(ssp_t ssp, uint8_t value);
static inline void ssp_write(ssp_t ssp, uint8_t value) { (void) ssp_readwrite(ssp, value); }
static inline uint8_t ssp_read(ssp_t ssp) { return ssp_readwrite(ssp, 0xff); }

END_DECL
