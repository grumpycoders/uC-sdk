#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

#include "hardware.h"

#include "gpio.h"

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

uint8_t gpio_irq_channels[] = {
    EXTI0_IRQn,
    EXTI1_IRQn,
    EXTI2_IRQn,
    EXTI3_IRQn,
    EXTI4_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI9_5_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn };

#define IRQ_EXTI_MAX 16
static void (*exti_irq_callback[IRQ_EXTI_MAX])();

static void call_exti_callback(int index) {
    if (exti_irq_callback[index]) {
        exti_irq_callback[index]();
    }
}

__attribute__((constructor)) static void irq_init() {
    int i;
    for (i = 0; i < IRQ_EXTI_MAX; i++)
        exti_irq_callback[i] = NULL;
}

void EXTI0_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        call_exti_callback(0);
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        call_exti_callback(1);
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI2_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
        call_exti_callback(2);
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}

void EXTI3_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        call_exti_callback(3);
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

void EXTI4_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        call_exti_callback(4);
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}

void EXTI9_5_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
        call_exti_callback(5);
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
        call_exti_callback(6);
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
        call_exti_callback(7);
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if (EXTI_GetITStatus(EXTI_Line8) != RESET) {
        call_exti_callback(8);
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
        call_exti_callback(9);
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void EXTI15_10_IRQHandler() {
    if (EXTI_GetITStatus(EXTI_Line10) != RESET) {
        call_exti_callback(10);
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    else if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
        call_exti_callback(11);
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    else if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
        call_exti_callback(12);
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    else if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
        call_exti_callback(13);
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    else if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
        call_exti_callback(14);
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    else if (EXTI_GetITStatus(EXTI_Line15) != RESET) {
        call_exti_callback(15);
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

void gpio_irq_init(pin_t pin, void (*cb)(), irq_trigger_t tt)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    SYSCFG_EXTILineConfig(pin.port, pin.pin);

    EXTI_InitTypeDef exti;
    exti.EXTI_Line = 1 << pin.pin;
    exti.EXTI_LineCmd = ENABLE;
    exti.EXTI_Mode = EXTI_Mode_Interrupt;
    switch(tt)
    {
        case rising:
            exti.EXTI_Trigger = EXTI_Trigger_Rising;
            break;
        case falling:
            exti.EXTI_Trigger = EXTI_Trigger_Falling;
            break;
        case change:
            exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
            break;
    }
    EXTI_Init(&exti);

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = gpio_irq_channels[pin.pin];
    nvic.NVIC_IRQChannelPreemptionPriority = 0x01;
    nvic.NVIC_IRQChannelSubPriority = 0x01;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    exti_irq_callback[pin.pin] = cb;
}

void gpio_irq_deinit(pin_t pin)
{
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = 1 << pin.pin;
    exti.EXTI_LineCmd = DISABLE;
    EXTI_Init(&exti);

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = gpio_irq_channels[pin.pin];
    nvic.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&nvic);

    exti_irq_callback[pin.pin] = NULL;
}
