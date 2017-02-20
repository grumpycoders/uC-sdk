#include "timer.h"

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>

#include <hardware.h>

static TIM_TypeDef *const timers[] = { (void *) 0, TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14, TIM15, TIM16, TIM17 };

void timer_config(timer_t timer, uint16_t prescale, uint32_t period)
{
    TIM_TypeDef *id = timers[timer];

    //all timers are 16 bits
    if (timer < timer_1 || timer > timer_17 || period > 0xffff)
        return;

    //clock the timer
    switch(timer)
    {
        case timer_1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
            break;
        case timer_2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            break;
        case timer_3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            break;
        case timer_4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            break;
        case timer_5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            break;
        case timer_6:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
            break;
        case timer_7:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
            break;
        case timer_8:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
            break;
        case timer_9:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
            break;
        case timer_10:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
            break;
        case timer_11:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
            break;
        case timer_12:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
            break;
        case timer_13:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
            break;
        case timer_14:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
            break;
        case timer_15:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
            break;
        case timer_16:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
            break;
        case timer_17:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
            break;
        default:
            return;
    }

    TIM_TimeBaseInitTypeDef def;
    def.TIM_Prescaler = prescale;
    def.TIM_CounterMode = TIM_OCMode_PWM1;
    def.TIM_Period = period;
    def.TIM_ClockDivision = TIM_CKD_DIV1;
    def.TIM_RepetitionCounter = 0;          //TIM1 and TIM8 only
    TIM_TimeBaseInit(id, &def);

    TIM_Cmd(id, ENABLE);
}

uint32_t timer_get_count(timer_t timer)
{
    return TIM_GetCounter(timers[timer]);
}

void timer_set_count(timer_t timer, uint32_t value)
{
    TIM_SetCounter(timers[timer], value);
}

uint32_t timer_get_clock_freq(timer_t timer)
{
    RCC_ClocksTypeDef def;
    RCC_GetClocksFreq(&def);

    if (timer == 1 || (timer >= 8 || timer <= 11) || timer >= 15)
        return def.PCLK2_Frequency;
    else
        return def.PCLK1_Frequency;
}

void timer_pwmchannel_init(timer_channel_t timer_port, pin_t pin, uint32_t pulse)
{
    timer_t timer = timer_port.timer;
    uint8_t channel = timer_port.channel;

    TIM_TypeDef *id = timers[timer];

    if (timer < 1 || timer > 17 || channel < 1 || channel > 4)
        return;

    //all timers are 16bits
    if (pulse > 0xffff)
        return;

    //clock the pin
    RCC_APB2PeriphClockCmd(1 << (pin.port + 2), ENABLE);

    //TODO: handle remaps
    gpio_config_alternate(pin, pin_dir_write, pull_none, 0);

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
            TIM_OC1Init(id, &def);
            TIM_OC1PreloadConfig(id, TIM_OCPreload_Enable);
            break;
        case 2:
            TIM_OC2Init(id, &def);
            TIM_OC2PreloadConfig(id, TIM_OCPreload_Enable);
            break;
        case 3:
            TIM_OC3Init(id, &def);
            TIM_OC3PreloadConfig(id, TIM_OCPreload_Enable);
            break;
        case 4:
            TIM_OC4Init(id, &def);
            TIM_OC4PreloadConfig(id, TIM_OCPreload_Enable);
            break;
    }
}
