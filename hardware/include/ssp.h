#pragma once

#include <stdint.h>
#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    ssp_port_0,
    ssp_port_1,
} ssp_t;

void ssp_config(ssp_t ssp, uint32_t clock);
void ssp_write(ssp_t ssp, uint8_t value);
uint8_t ssp_read(ssp_t ssp);
