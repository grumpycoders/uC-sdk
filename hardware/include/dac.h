#pragma once

#include <decl.h>
#include <stdint.h>

#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    dac_0,
    dac_1,
    dac_2,
    dac_3,
} dac_t;

typedef struct {
    pin_t pin;
    dac_t dac;
} dac_port_t;

BEGIN_DECL

void dac_config(dac_port_t dac_port);

//set single value
void dac_set(dac_t dac, uint8_t value);

//set single value
void dac_set16(dac_t dac, uint16_t value);

END_DECL
