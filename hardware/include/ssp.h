#pragma once

#include <decl.h>
#include <stdint.h>

#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    ssp_0,
    ssp_1,
    ssp_2,
    ssp_3,
    ssp_4,
    ssp_5,
    ssp_6,
} ssp_t;

typedef struct {
    pin_t sclk;
    pin_t mosi;
    pin_t miso;
    ssp_t ssp;
} ssp_port_t;

#ifdef __cplusplus
static_assert(sizeof(ssp_port_t) <= 8, "ssp_port_t isn't 64 bits-wide");
#else
_Static_assert(sizeof(ssp_port_t) <= 8, "ssp_port_t isn't 64 bits-wide");
#endif

BEGIN_DECL

void ssp_config(ssp_port_t ssp_port, uint32_t clock);
uint8_t ssp_readwrite(ssp_t ssp, uint8_t value);
static __inline__ void ssp_write(ssp_t ssp, uint8_t value) { (void) ssp_readwrite(ssp, value); }
static __inline__ uint8_t ssp_read(ssp_t ssp) { return ssp_readwrite(ssp, 0xff); }

END_DECL
