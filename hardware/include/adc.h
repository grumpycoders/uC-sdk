#pragma once

#include <decl.h>
#include <stdint.h>

#include <gpio.h>

// ought to be generic, cm3-specific for now
typedef enum {
    adc_0,
    adc_1,
    adc_2,
    adc_3,
} adc_t;

BEGIN_DECL

void adc_calibrate(uint8_t adc);

//configure all ADC
void adc_config_all();

//configure single shot measuring
void adc_config_single(adc_t adc, uint8_t channel, pin_t pin);

//configure continuous measuring
void adc_config_continuous(adc_t adc, uint8_t *channel, pin_t *pin, uint16_t *dest, uint8_t nb);

//get single measure
uint16_t adc_get(adc_t adc);

END_DECL
