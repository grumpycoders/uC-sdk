#pragma once

#include <decl.h>
#include <gpio.h>
#include <ssp.h>

typedef struct {
    // these are to be filled by caller
    pin_t cs;
    // private data
    ssp_t ssp;
    uint8_t odr:4, scale:3, power:1;
} lis3dsh_t;

BEGIN_DECL

int lis3dsh_init_ssp(lis3dsh_t * lis3dsh, ssp_port_t ssp_port);
void lis3dsh_power(lis3dsh_t * lis3dsh, int power);
void lis3dsh_scale(lis3dsh_t * lis3dsh, int scale);
void lis3dsh_frequency(lis3dsh_t * lis3dsh, int odr);
void lis3dsh_read(lis3dsh_t * lis3dsh, float axis[3]);

END_DECL
