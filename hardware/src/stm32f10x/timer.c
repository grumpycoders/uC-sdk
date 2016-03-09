#include "timer.h"

#include <hardware.h>

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>


struct timerInitDef_t {
    TIM_TypeDef * id;
    volatile uint32_t * bridge;
    uint32_t peripheral;
};

static struct timerInitDef_t timerInitDefs[] = {
    { TIM1,  &RCC->APB2ENR, RCC_APB2Periph_TIM1 },
    { TIM2,  &RCC->APB1ENR, RCC_APB1Periph_TIM2 },
    { TIM3,  &RCC->APB1ENR, RCC_APB1Periph_TIM3 },
    { TIM4,  &RCC->APB1ENR, RCC_APB1Periph_TIM4 },
    { TIM5,  &RCC->APB1ENR, RCC_APB1Periph_TIM5 },
    { TIM6,  &RCC->APB1ENR, RCC_APB1Periph_TIM6 },
    { TIM7,  &RCC->APB1ENR, RCC_APB1Periph_TIM7 },
    { TIM8,  &RCC->APB2ENR, RCC_APB2Periph_TIM8 },
    { TIM9,  &RCC->APB2ENR, RCC_APB2Periph_TIM9 },
    { TIM10, &RCC->APB2ENR, RCC_APB2Periph_TIM10 },
    { TIM11, &RCC->APB2ENR, RCC_APB2Periph_TIM11 },
    { TIM12, &RCC->APB1ENR, RCC_APB1Periph_TIM12 },
    { TIM13, &RCC->APB1ENR, RCC_APB1Periph_TIM13 },
    { TIM14, &RCC->APB1ENR, RCC_APB1Periph_TIM14 },
    { TIM15, &RCC->APB2ENR, RCC_APB2Periph_TIM15 },
    { TIM16, &RCC->APB2ENR, RCC_APB2Periph_TIM16 },
    { TIM17, &RCC->APB2ENR, RCC_APB2Periph_TIM17 }
};

uint32_t timer_get_freq(uint8_t timer)
{
    RCC_ClocksTypeDef def;
    RCC_GetClocksFreq(&def);

    if (timer == 1 || (timer >= 8 || timer <= 11) || timer >= 15)
        return def.PCLK2_Frequency;
    else
        return def.PCLK1_Frequency;
}

void timer_init(uint8_t timer, uint8_t channel, uint16_t prescale, uint32_t period)
{
    if (timer < 1 || timer > 17 || channel < 1 || channel > 4)
        return;

    //all timers are 16bits
    if (period > 0xffff)
        return;

    struct timerInitDef_t * timerInitDef = timerInitDefs + timer - 1;

    //clock the timer
    *(timerInitDef->bridge) |= timerInitDef->peripheral;

    TIM_TimeBaseInitTypeDef def;
    def.TIM_Prescaler = prescale;
    def.TIM_CounterMode = TIM_OCMode_PWM1;
    def.TIM_Period = period;
    def.TIM_ClockDivision = TIM_CKD_DIV1;
    def.TIM_RepetitionCounter = 0;			//TIM1 and TIM8 only
    TIM_TimeBaseInit(timerInitDef->id, &def);

    TIM_Cmd(timerInitDef->id, ENABLE);
}

void timer_init_pwmchannel(uint8_t timer, uint8_t channel, pin_t pin, uint32_t pulse)
{
    if (timer < 1 || timer > 17 || channel < 1 || channel > 4)
        return;

    //all timers are 16bits
    if (pulse > 0xffff)
        return;

   struct timerInitDef_t * timerInitDef = timerInitDefs + timer - 1;

    //clock the pin
    RCC_APB2PeriphClockCmd(1 << (pin.port + 2), ENABLE);

    //setup the GPIO pin
    GPIO_InitTypeDef gpiodef;
    gpiodef.GPIO_Pin = 1 << pin.pin;
    gpiodef.GPIO_Mode = GPIO_Mode_AF_PP;
    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(stm32f10x_gpio_ports[pin.port], &gpiodef);

   //setup the Timer channel
    TIM_OCInitTypeDef def;
    def.TIM_OCMode = TIM_OCMode_PWM1;
    def.TIM_OutputState = TIM_OutputState_Enable;
    def.TIM_OutputNState = TIM_OutputState_Enable; //Only TIM1 and TIM8
    def.TIM_Pulse = pulse;
    def.TIM_OCPolarity = TIM_OCPolarity_High;
    def.TIM_OCNPolarity = TIM_OCPolarity_High;     //Only TIM1 and TIM8
    def.TIM_OCIdleState = TIM_OCPolarity_High;
    def.TIM_OCNIdleState = TIM_OCPolarity_High;    //Only TIM1 and TIM8

    switch(channel)
    {
        case 1:
            TIM_OC1Init(timerInitDef->id, &def);
            TIM_OC1PreloadConfig(timerInitDef->id, TIM_OCPreload_Enable);
            break;
        case 2:
            TIM_OC2Init(timerInitDef->id, &def);
            TIM_OC2PreloadConfig(timerInitDef->id, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3Init(timerInitDef->id, &def);
            TIM_OC3PreloadConfig(timerInitDef->id, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4Init(timerInitDef->id, &def);
            TIM_OC4PreloadConfig(timerInitDef->id, TIM_OCPreload_Enable);
            break;
    }
}
