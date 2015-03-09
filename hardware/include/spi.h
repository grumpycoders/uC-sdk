#pragma once

#include <decl.h>
#include <stdint.h>
#include <ssp.h>

BEGIN_DECL

static inline void spi_read_memory(ssp_t ssp, uint8_t address, uint8_t * buffer, uint8_t size) {
    ssp_write(ssp, address | 0x80);
    while (size--) *buffer++ = ssp_read(ssp);
}

static inline void spi_write_memory(ssp_t ssp, uint8_t address, const uint8_t * buffer, uint8_t size) {
    ssp_write(ssp, address);
    while (size--) ssp_write(ssp, *buffer++);
}

END_DECL

