#pragma once

#include <decl.h>
#include <gpio.h>
#include <ssp.h>

typedef struct {
    ssp_t ssp;
    pin_t cs;
    int got_timeout:1, error_state:1;
    // these should remain opaque
    int v2:1, sdhc:1;
} sdcard_t;

BEGIN_DECL

int sdcard_init(sdcard_t * sdcard, ssp_port_t ssp_port, pin_t cs);
int sdcard_read(sdcard_t * sdcard, uint8_t * data, unsigned int block);

END_DECL
