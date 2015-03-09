#pragma once

#include <gpio.h>
#include <ssp.h>
#include <i2c.h>


// ought to be generic, cm3-specific for now
typedef enum {
    l3gd20_i2c,
    l3gd20_spi,
} l3gd20_communication_t;

typedef struct {
    l3gd20_communication_t comm;
    ssp_port_t ssp;
//    i2c_port_t i2c;
    pin_t cs;
    float sensitivity;
} l3gd20_t;


l3gd20_t *l3gd20_init_ssp(ssp_port_t port, pin_t cs);
l3gd20_t *l3gd20_init_i2c(ssp_port_t port, pin_t cs);

void l3gd20_setup_filter(l3gd20_t *sensor, float cutoff);

int l3gd20_power_on(l3gd20_t *sensor, int power);

uint8_t l3gd20_who_am_i(l3gd20_t *sensor);

void l3gd20_read(l3gd20_t *sensor, float axis[3]);

void l3gd20_set_sensitivity(l3gd20_t *sensor);
void l3gd20_set_datarate(l3gd20_t *sensor);

