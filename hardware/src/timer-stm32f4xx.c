#include <stm32f4xx.h>
#include <stm32f4xx_tim.h>

#include "timer.h"

struct timerInitDef_t {
    TIM_TypeDef * id;
    uint8_t af;
    volatile uint32_t * bridge;
    uint32_t peripheral;
};

static struct timerInitDef_t timerInitDefs[] = {
    { TIM1, GPIO_AF_TIM1, &RCC->APB2ENR, RCC_APB2Periph_TIM1}, 
    { TIM2, GPIO_AF_TIM2, &RCC->APB1ENR, RCC_APB1Periph_TIM2}, 
    { TIM3, GPIO_AF_TIM3, &RCC->APB1ENR, RCC_APB1Periph_TIM3}, 
    { TIM4, GPIO_AF_TIM4, &RCC->APB1ENR, RCC_APB1Periph_TIM4}, 
    { TIM5, GPIO_AF_TIM5, &RCC->APB1ENR, RCC_APB1Periph_TIM5}, 
    { TIM6, 0, &RCC->APB1ENR, RCC_APB1Periph_TIM6}, 
    { TIM7, 0, &RCC->APB1ENR, RCC_APB1Periph_TIM7}, 
    { TIM8, GPIO_AF_TIM8, &RCC->APB2ENR, RCC_APB2Periph_TIM8}, 
    { TIM9, GPIO_AF_TIM9, &RCC->APB2ENR, RCC_APB2Periph_TIM9}, 
    { TIM10, GPIO_AF_TIM10, &RCC->APB2ENR, RCC_APB2Periph_TIM10}, 
    { TIM11, GPIO_AF_TIM11, &RCC->APB2ENR, RCC_APB2Periph_TIM11}, 
    { TIM12, GPIO_AF_TIM12, &RCC->APB1ENR, RCC_APB1Periph_TIM12}, 
    { TIM13, GPIO_AF_TIM13, &RCC->APB1ENR, RCC_APB1Periph_TIM13}, 
    { TIM14, GPIO_AF_TIM14, &RCC->APB1ENR, RCC_APB1Periph_TIM14}
};

extern GPIO_TypeDef *ports[];

void timer_init(uint8_t timer, uint8_t channel, uint16_t prescale, uint32_t period)
{
    if (timer < 1 || timer > 14 || channel < 1 || channel > 4)
        return;

    //only TIM2 and TIM5 are 32 bits
    if (timer != 2 && timer != 5 && period > 0xffff)
        return;

    struct timerInitDef_t * timerInitDef = timerInitDefs + timer - 1;

    //clock the timer
    *(timerInitDef->bridge) |= timerInitDef->peripheral;

    TIM_TimeBaseInitTypeDef def;
    def.TIM_Prescaler = prescale;
    def.TIM_CounterMode = TIM_CounterMode_Up;
    def.TIM_Period = period;
    def.TIM_ClockDivision = TIM_CKD_DIV1;
    def.TIM_RepetitionCounter = 0;			//TIM1 and TIM8 only
    TIM_TimeBaseInit(timerInitDef->id, &def);

    TIM_Cmd(timerInitDef->id, ENABLE);
}

void timer_init_pwmchannel(uint8_t timer, uint8_t channel, pin_t pin, uint16_t pulse)
{
    if (timer < 1 || timer > 14 || channel < 1 || channel > 4)
        return;

   struct timerInitDef_t * timerInitDef = timerInitDefs + timer - 1;

    //clock the pin
    RCC_AHB1PeriphClockCmd(1 << get_port(pin), ENABLE);

    //setup the GPIO pin
    GPIO_InitTypeDef gpiodef;
    gpiodef.GPIO_Pin = 1 << get_pin(pin);
    gpiodef.GPIO_Mode = GPIO_Mode_AF;
    gpiodef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ports[get_port(pin)], &gpiodef);

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

    GPIO_PinAFConfig(ports[get_port(pin)], get_pin(pin), timerInitDef->af);
}


