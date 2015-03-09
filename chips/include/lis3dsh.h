#pragma once

#include <decl.h>
#include <gpio.h>
#include <ssp.h>

typedef struct {
    // these are to be filled by caller
    ssp_port_t ssp_port;
    pin_t cs;
} lis3dsh_t;

BEGIN_DECL

int lis3dsh_init(lis3dsh_t * lis3dsh);
void lis3dsh_power(lis3dsh_t * lis3dsh, int power);
void lis3dsh_read(lis3dsh_t * lis3dsh, float axis[3]);

END_DECL
