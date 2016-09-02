#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <hardware.h>

#include <gpio.h>

GPIO_TypeDef * const stm32f4xx_gpio_ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK };

void gpio_config(pin_t pin, pin_dir_t dir, pull_t pull) {
    //Clock the port
    RCC_AHB1PeriphClockCmd(1 << pin.port, ENABLE);

    GPIO_InitTypeDef def;
    def.GPIO_Pin = 1 << pin.pin;
    def.GPIO_Mode = dir;
    def.GPIO_Speed = GPIO_Speed_100MHz;
    if (dir)
        def.GPIO_OType = GPIO_OType_PP; //output : Push Pull
    else
        def.GPIO_OType = GPIO_OType_OD; //input : Open Drain
    def.GPIO_PuPd = pull;

    GPIO_Init(stm32f4xx_gpio_ports[pin.port], &def);
}

void gpio_config_alternate(pin_t pin, pin_dir_t dir, pull_t pull, uint8_t af) {
    if (af > 15)
        return;
    //Clock the port
    RCC_AHB1PeriphClockCmd(1 << pin.port, ENABLE);

    GPIO_InitTypeDef def;
    def.GPIO_Pin = 1 << pin.pin;
    def.GPIO_Mode = GPIO_Mode_AF;
    def.GPIO_Speed = GPIO_Speed_100MHz;
    if (dir)
        def.GPIO_OType = GPIO_OType_PP; //output : Push Pull
    else
        def.GPIO_OType = GPIO_OType_OD; //input : Open Drain
    def.GPIO_PuPd = pull;

    GPIO_Init(stm32f4xx_gpio_ports[pin.port], &def);
    GPIO_PinAFConfig(stm32f4xx_gpio_ports[pin.port], pin.pin, af);
}

void gpio_set(pin_t pin, int enabled) {
    if (enabled)
        GPIO_SetBits(stm32f4xx_gpio_ports[pin.port], 1 << pin.pin);
    else
        GPIO_ResetBits(stm32f4xx_gpio_ports[pin.port], 1 << pin.pin);
}

uint8_t gpio_get(pin_t pin) {
    return GPIO_ReadInputDataBit(stm32f4xx_gpio_ports[pin.port], 1 << pin.pin);
}
