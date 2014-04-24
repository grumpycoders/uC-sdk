#pragma once

#include <decl.h>
#include <gpio.h>
#include <ssp.h>

typedef struct {
    // these are to be filled by caller
    pin_t cs;
    ssp_t ssp;
    // these are read-only publics
    int got_timeout;
    // these should remain opaque
} sdcard_t;

BEGIN_DECL

int sdcard_init(sdcard_t * sdcard);

END_DECL
