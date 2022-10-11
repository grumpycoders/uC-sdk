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

typedef enum {
    ssp_slave,
    ssp_master,
} ssp_mode_t;

typedef enum {
    ssp_polarity_mode_0, // CPOL=0; CPHA=0
    ssp_polarity_mode_1, // CPOL=0; CPHA=1
    ssp_polarity_mode_2, // CPOL=1; CPHA=0
    ssp_polarity_mode_3, // CPOL=1; CPHA=1
} ssp_polarity_t;

typedef enum {
    event_read,
    event_write,
}irq_ssp_event_t;

typedef struct {
    pin_t sclk;
    pin_t mosi;
    pin_t miso;
    pin_t ss;
    ssp_mode_t mode;
    ssp_polarity_t polarity;
    ssp_t ssp;
} ssp_port_t;

ucsdk_static_assert(sizeof(ssp_port_t) <= 11, "ssp_port_t isn't 88 bits-wide");

BEGIN_DECL

void ssp_config(ssp_port_t ssp_port, uint32_t clock);
uint8_t ssp_readwrite(ssp_t ssp, uint8_t value);
static __inline__ void ssp_write(ssp_t ssp, uint8_t value) { (void) ssp_readwrite(ssp, value); }
static __inline__ uint8_t ssp_read(ssp_t ssp) { return ssp_readwrite(ssp, 0xff); }

void ssp_irq_init(ssp_t ssp, irq_ssp_event_t event, void (*cb)());
void ssp_slave_start_read(ssp_t ssp);
void ssp_slave_stop_read(ssp_t ssp);
void ssp_slave_start_write(ssp_t ssp);
void ssp_slave_stop_write(ssp_t ssp);

END_DECL
