#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>

#include "gpio.h"

static inline uint8_t getPort(pin_t pin) {
    return (pin >> 8) && 0xff;
}

static inline uint8_t getPin(pin_t pin) {
    return pin & 0xff;
}

void gpio_config(pin_t pin, pin_dir_t dir) {
    PINSEL_CFG_Type pinCfg;
    pinCfg.Portnum = getPort(pin);
    pinCfg.Pinnum = getPin(pin);
    pinCfg.Funcnum = 0;
    pinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
    pinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
    PINSEL_ConfigPin(&pinCfg);
    FIO_SetDir(getPort(pin), 1 << getPin(pin), dir);
}

void gpio_set(pin_t pin, int enabled) {
    if (enabled)
        FIO_SetValue(getPort(pin), 1 << getPin(pin));
    else
        FIO_ClearValue(getPort(pin), 1 << getPin(pin));
}

int gpio_get(pin_t pin) {
    return (FIO_ReadValue(getPort(pin)) & (1 << getPin(pin))) ? 1 : 0;
}
