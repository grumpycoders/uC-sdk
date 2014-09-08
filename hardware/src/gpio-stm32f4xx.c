#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

#include "gpio.h"

static inline uint8_t get_port(pin_t pin) {
    return (pin >> 8) & 0xff;
}

static inline uint8_t get_pin(pin_t pin) {
    return pin & 0xff;
}

GPIO_TypeDef *ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK};

void gpio_config(pin_t pin, pin_dir_t dir) { 
    GPIO_InitTypeDef def;
    def.GPIO_Pin = 1 << get_pin(pin);
    def.GPIO_Mode = dir;
    def.GPIO_Speed = GPIO_Speed_50MHz;
    def.GPIO_OType = GPIO_OType_PP;   //input : Open Drain, output : Push Pull
    def.GPIO_PuPd = GPIO_PuPd_UP;     //Pull up resistor

    //Clock the port
    RCC_AHB1PeriphClockCmd(1 << get_port(pin), ENABLE);

    GPIO_Init(ports[get_port(pin)], &def);
}

void gpio_set(pin_t pin, int enabled) {
    if (enabled)
        GPIO_SetBits(ports[get_port(pin)], 1 << get_pin(pin));
    else
        GPIO_ResetBits(ports[get_port(pin)], 1 << get_pin(pin));
}

int gpio_get(pin_t pin) {
    return GPIO_ReadInputDataBit(ports[get_port(pin)], 1 << get_pin(pin));
}
