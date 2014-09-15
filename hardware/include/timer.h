#pragma once

#include <decl.h>
#include <stdint.h>

#include <gpio.h>

BEGIN_DECL

uint32_t timer_get_freq(uint8_t timer);
void timer_init(uint8_t timer, uint8_t channel, uint16_t prescale, uint32_t period);
void timer_init_pwmchannel(uint8_t timer, uint8_t channel, pin_t pin, uint32_t pulse);

END_DECL
