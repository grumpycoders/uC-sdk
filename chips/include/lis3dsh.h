#pragma once

#include <decl.h>
#include <gpio.h>
#include <ssp.h>

typedef struct {
    pin_t cs;
    ssp_t ssp;
    uint8_t odr:4, scale:3, power:1;
} lis3dsh_t;

typedef enum {
    LIS3DSH_POWER_OFF   = 0,
    LIS3DSH_POWER_ON    = 1,
} lis3dsh_power_t;

typedef enum {
    LIS3DSH_SCALE_2G    = 0,
    LIS3DSH_SCALE_4G    = 1,
    LIS3DSH_SCALE_6G    = 2,
    LIS3DSH_SCALE_8G    = 3,
    LIS3DSH_SCALE_16G   = 4,
} lis3dsh_scale_t;

typedef enum {
    LIS3DSH_ODR_3_125HZ = 1,
    LIS3DSH_ODR_6_25HZ  = 2,
    LIS3DSH_ODR_12_5HZ  = 3,
    LIS3DSH_ODR_25HZ    = 4,
    LIS3DSH_ODR_50HZ    = 5,
    LIS3DSH_ODR_100HZ   = 6,
    LIS3DSH_ODR_400HZ   = 7,
    LIS3DSH_ODR_800HZ   = 8,
    LIS3DSH_ODR_1600HZ  = 9,
} lis3dsh_odr_t;

BEGIN_DECL

int lis3dsh_init_ssp(lis3dsh_t * lis3dsh, ssp_port_t ssp_port, pin_t cs);
void lis3dsh_power(lis3dsh_t * lis3dsh, lis3dsh_power_t power);
void lis3dsh_scale(lis3dsh_t * lis3dsh, lis3dsh_scale_t scale);
void lis3dsh_frequency(lis3dsh_t * lis3dsh, lis3dsh_odr_t odr);
void lis3dsh_read(lis3dsh_t * lis3dsh, float axis[3]);

END_DECL
