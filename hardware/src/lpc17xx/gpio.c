#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>

#include "gpio.h"

void gpio_config(pin_t pin, pin_dir_t dir, pull_t pull) {
    PINSEL_CFG_Type pin_cfg;
    pin_cfg.Portnum = pin.port;
    pin_cfg.Pinnum = pin.pin;
    pin_cfg.Funcnum = 0;

    switch (pull) {
    case pull_none:
        pin_cfg.Pinmode = PINSEL_PINMODE_TRISTATE;
        break;
    case pull_up:
        pin_cfg.Pinmode = PINSEL_PINMODE_PULLUP;
        break;
    case pull_down:
        pin_cfg.Pinmode = PINSEL_PINMODE_PULLDOWN;
        break;
    }
    pin_cfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&pin_cfg);
    FIO_SetDir(pin.port, 1 << pin.pin, dir);
}

void gpio_set(pin_t pin, int enabled) {
    if (enabled)
        FIO_SetValue(pin.port, 1 << pin.pin);
    else
        FIO_ClearValue(pin.port, 1 << pin.pin);
}

uint8_t gpio_get(pin_t pin) {
    return (FIO_ReadValue(pin.port) & (1 << pin.pin)) ? 1 : 0;
}
