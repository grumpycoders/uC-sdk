#pragma once

#include <decl.h>
#include <stdint.h>

#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    timer_0,
    timer_1,
    timer_2,
    timer_3,
    timer_4,
    timer_5,
    timer_6,
    timer_7,
    timer_8,
    timer_9,
    timer_10,
    timer_11,
    timer_12,
    timer_13,
    timer_14,
    timer_15,
} timer_t;

typedef enum {
    event_update,
    event_commutation,
    event_trigger,
    event_break,
    event_output_capture,
}irq_timer_event_t;

typedef struct {
    timer_t timer;
    uint8_t channel;
} timer_channel_t;

_Static_assert(sizeof(timer_channel_t) <= 4, "timer_channel_t isn't 32 bits-wide");

BEGIN_DECL

void timer_config(timer_t timer, uint16_t prescale, uint32_t period);

uint32_t timer_get_clock_freq(timer_t timer);

//void timer_init(uint8_t timer, uint8_t channel, uint16_t prescale, uint32_t period);
void timer_pwmchannel_init(timer_channel_t timer_port, pin_t pin, uint32_t pulse);

void timer_irq_init(timer_channel_t timer_port, irq_timer_event_t event, void (*cb)());
void timer_irq_deinit(timer_t timer, irq_timer_event_t event);

END_DECL
