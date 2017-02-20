#include "timer.h"

#include <stm32f4xx.h>
#include <stm32f4xx_tim.h>

#include <stddef.h>

#include "hardware.h"

static TIM_TypeDef *const timers[] = { (void *) 0, TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14 };

void timer_config(timer_t timer, uint16_t prescale, uint32_t period)
{
    TIM_TypeDef *id = timers[timer];

    if (timer < timer_1 || timer > timer_14)
        return;

    //only TIM2 and TIM5 are 32 bits
    if (timer != 2 && timer != 5 && period > 0xffff)
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
        default:
            return;
    }

    TIM_TimeBaseInitTypeDef def;
    def.TIM_Prescaler = prescale;
    def.TIM_CounterMode = TIM_CounterMode_Up;
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

    if (timer == timer_1 || (timer >= timer_8|| timer <= timer_11))
        return def.PCLK2_Frequency;
    else
        return def.PCLK1_Frequency;
}

void timer_pwmchannel_init(timer_channel_t timer_port, pin_t pin, uint32_t pulse)
{
    timer_t timer = timer_port.timer;
    uint8_t channel = timer_port.channel;

    TIM_TypeDef *id = timers[timer];

    if (timer < 1 || timer > 14 || timer == 6 || timer == 7 || channel < 1 || channel > 4)
        return;

    //only TIM2 and TIM5 are 32 bits
    if (timer != 2 && timer != 5 && pulse > 0xffff)
        return;

   if (timer == 1 || timer == 2)
       gpio_config_alternate(pin, pin_dir_write, pull_none, 1);
   else if (timer >= 3 && timer <= 5)
       gpio_config_alternate(pin, pin_dir_write, pull_none, 2);
   else if (timer >= 8 && timer <= 11)
       gpio_config_alternate(pin, pin_dir_write, pull_none, 3);
   else if (timer >= 9 && timer <= 14)
       gpio_config_alternate(pin, pin_dir_write, pull_none, 9);

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
        default:
            return;
    }
}



uint8_t irq_timer_channels[] = {
//  event_update                event_commutation           event_trigger               event_break             event_output_capture
    TIM1_UP_TIM10_IRQn,         TIM1_TRG_COM_TIM11_IRQn,    TIM1_TRG_COM_TIM11_IRQn,    TIM1_BRK_TIM9_IRQn,     TIM1_CC_IRQn,
    TIM2_IRQn,                  0,                          TIM2_IRQn,                  0,                      TIM2_IRQn,
    TIM3_IRQn,                  0,                          TIM3_IRQn,                  0,                      TIM3_IRQn,
    TIM4_IRQn,                  0,                          TIM4_IRQn,                  0,                      TIM4_IRQn,
    TIM5_IRQn,                  0,                          TIM5_IRQn,                  0,                      TIM5_IRQn,
    TIM6_DAC_IRQn,              0,                          0,                          0,                      TIM6_DAC_IRQn,
    TIM7_IRQn,                  0,                          0,                          0,                      TIM7_IRQn,
    TIM8_UP_TIM13_IRQn,         TIM8_TRG_COM_TIM14_IRQn,    TIM8_TRG_COM_TIM14_IRQn,    TIM8_BRK_TIM12_IRQn,    TIM8_CC_IRQn,
    TIM1_BRK_TIM9_IRQn,         0,                          TIM1_BRK_TIM9_IRQn,         0,                      TIM1_BRK_TIM9_IRQn,
    TIM1_UP_TIM10_IRQn,         0,                          0,                          0,                      TIM1_UP_TIM10_IRQn,
    TIM1_TRG_COM_TIM11_IRQn,    0,                          0,                          0,                      TIM1_TRG_COM_TIM11_IRQn,
    TIM8_BRK_TIM12_IRQn,        0,                          TIM8_BRK_TIM12_IRQn,        0,                      TIM8_BRK_TIM12_IRQn,
    TIM8_UP_TIM13_IRQn,         0,                          0,                          0,                      TIM8_UP_TIM13_IRQn,
    TIM8_TRG_COM_TIM14_IRQn,    0,                          0,                          0,                      TIM8_TRG_COM_TIM14_IRQn,
};

//TODO: independant timer for each event
#define IRQ_TIMER_MAX (14 * 5)
static void (*irq_timer_callback[IRQ_TIMER_MAX])();

__attribute__((constructor)) static void timer_irq_callback_init() {
    int i;
    for (i = 0; i < IRQ_TIMER_MAX; i++)
        irq_timer_callback[i] = NULL;
}

static void set_timer_callback(timer_t timer, irq_timer_event_t event, void (*cb)()) {
    irq_timer_callback[timer * 5 + event] = cb;
}

static void call_timer_callback(timer_t timer, irq_timer_event_t event) {
    if (irq_timer_callback[timer * 5 + event]) {
        irq_timer_callback[timer * 5 + event]();
    }
}

void TIM1_BRK_TIM9_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_Break) != RESET){
        call_timer_callback(timer_1, event_break);
        TIM_ClearITPendingBit(TIM1, TIM_IT_Break);
    }
    else if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET){
        call_timer_callback(timer_9, event_update);
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM9, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_9, event_trigger);
        TIM_ClearITPendingBit(TIM9, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM9, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_9, event_output_capture);
        TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM9, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_9, event_output_capture);
        TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM9, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_9, event_output_capture);
        TIM_ClearITPendingBit(TIM9, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM9, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_9, event_output_capture);
        TIM_ClearITPendingBit(TIM9, TIM_IT_CC4);
    }
}

void TIM1_UP_TIM10_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET){
        call_timer_callback(timer_1, event_update);
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET){
        call_timer_callback(timer_10, event_update);
        TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_10, event_output_capture);
        TIM_ClearITPendingBit(TIM10, TIM_IT_CC1);
    }
}

void TIM1_TRG_COM_TIM11_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_1, event_trigger);
        TIM_ClearITPendingBit(TIM1, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM1, TIM_IT_COM) != RESET){
        call_timer_callback(timer_1, event_commutation);
        TIM_ClearITPendingBit(TIM1, TIM_IT_COM);
    }
    else if (TIM_GetITStatus(TIM11, TIM_IT_Update) != RESET){
        call_timer_callback(timer_11, event_update);
        TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM11, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_11, event_output_capture);
        TIM_ClearITPendingBit(TIM11, TIM_IT_CC1);
    }
}

void TIM1_CC_IRQHandler() {
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_1, event_output_capture);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_1, event_output_capture);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_1, event_output_capture);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_1, event_output_capture);
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC4);
    }
}

void TIM2_IRQHandler() {
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
        call_timer_callback(timer_2, event_update);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_2, event_trigger);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_2, event_output_capture);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_2, event_output_capture);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_2, event_output_capture);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_2, event_output_capture);
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    }
}

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
        call_timer_callback(timer_3, event_update);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_3, event_trigger);
        TIM_ClearITPendingBit(TIM3, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_3, event_output_capture);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_3, event_output_capture);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_3, event_output_capture);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_3, event_output_capture);
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
    }
}

void TIM4_IRQHandler() {
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
        call_timer_callback(timer_4, event_update);
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM4, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_4, event_trigger);
        TIM_ClearITPendingBit(TIM4, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_4, event_output_capture);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_4, event_output_capture);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_4, event_output_capture);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_4, event_output_capture);
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);
    }
}

void TIM5_IRQHandler() {
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
        call_timer_callback(timer_5, event_update);
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM5, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_5, event_trigger);
        TIM_ClearITPendingBit(TIM5, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_5, event_output_capture);
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_5, event_output_capture);
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_5, event_output_capture);
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_5, event_output_capture);
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);
    }
}

void TIM7_IRQHandler() {
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){
        call_timer_callback(timer_6, event_update);
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

void TIM8_BRK_TIM12_IRQHandler() {
    if (TIM_GetITStatus(TIM8, TIM_IT_Break) != RESET){
        call_timer_callback(timer_8, event_break);
        TIM_ClearITPendingBit(TIM8, TIM_IT_Break);
    }
    else if (TIM_GetITStatus(TIM12, TIM_IT_Update) != RESET){
        call_timer_callback(timer_12, event_update);
        TIM_ClearITPendingBit(TIM12, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM12, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_12, event_trigger);
        TIM_ClearITPendingBit(TIM12, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM12, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_12, event_output_capture);
        TIM_ClearITPendingBit(TIM12, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM12, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_12, event_output_capture);
        TIM_ClearITPendingBit(TIM12, TIM_IT_CC2);
    }
}

void TIM8_UP_TIM13_IRQHandler() {
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET){
        call_timer_callback(timer_8, event_update);
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM13, TIM_IT_Update) != RESET){
        call_timer_callback(timer_13, event_update);
        TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM13, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_13, event_output_capture);
        TIM_ClearITPendingBit(TIM13, TIM_IT_CC1);
    }
}

void TIM8_TRG_COM_TIM14_IRQHandler() {
    if (TIM_GetITStatus(TIM8, TIM_IT_Trigger) != RESET){
        call_timer_callback(timer_8, event_trigger);
        TIM_ClearITPendingBit(TIM8, TIM_IT_Trigger);
    }
    else if (TIM_GetITStatus(TIM8, TIM_IT_COM) != RESET){
        call_timer_callback(timer_8, event_commutation);
        TIM_ClearITPendingBit(TIM8, TIM_IT_COM);
    }
    else if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET){
        call_timer_callback(timer_14, event_update);
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
    else if (TIM_GetITStatus(TIM14, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_14, event_output_capture);
        TIM_ClearITPendingBit(TIM14, TIM_IT_CC1);
    }
}

void TIM8_CC_IRQHandler() {
    if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET){
        call_timer_callback(timer_8, event_output_capture);
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
    }
    else if (TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET){
        call_timer_callback(timer_8, event_output_capture);
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
    }
    else if (TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET){
        call_timer_callback(timer_8, event_output_capture);
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
    }
    else if (TIM_GetITStatus(TIM8, TIM_IT_CC4) != RESET){
        call_timer_callback(timer_8, event_output_capture);
        TIM_ClearITPendingBit(TIM8, TIM_IT_CC4);
    }
}

void timer_irq_init(timer_channel_t timer_port, irq_timer_event_t event, void (*cb)()){
    timer_t timer = timer_port.timer;
    uint8_t channel = timer_port.channel;

    TIM_TypeDef *id = timers[timer];

    if (timer < timer_1 || timer > timer_14 || channel < 1 || channel > 4)
        return;

    switch(event){
        case event_update:
            TIM_ClearITPendingBit(id, TIM_IT_Update);
            TIM_ITConfig(id, TIM_IT_Update, ENABLE);
        break;
        case event_commutation:
            if (timer != timer_1 && timer != timer_8)
                return;
            TIM_ClearITPendingBit(id, TIM_IT_COM);
            TIM_ITConfig(id, TIM_IT_COM, ENABLE);
            break;
        case event_trigger:
            if (timer == timer_6 || timer == timer_7 ||
                timer == timer_10 || timer == timer_11 ||
                timer == timer_13 || timer == timer_14)
                return;
            TIM_ClearITPendingBit(id, TIM_IT_Trigger);
            TIM_ITConfig(id, TIM_IT_Trigger, ENABLE);
            break;
        case event_break:
            if (timer != timer_1 && timer != timer_8)
                return;
            TIM_ITConfig(id, TIM_IT_Break, ENABLE);
            break;
        case event_output_capture:
            switch(channel){
                case 1:
                    if (timer == timer_6 || timer == timer_7)
                        return;
                    TIM_ClearITPendingBit(id, TIM_IT_CC1);
                    TIM_ITConfig(id, TIM_IT_CC1, ENABLE);
                    break;
                case  2:
                    if (timer == timer_6 || timer == timer_7 ||
                        timer == timer_10 || timer == timer_11 ||
                        timer == timer_13 || timer == timer_14)
                        return;
                    TIM_ClearITPendingBit(id, TIM_IT_CC2);
                    TIM_ITConfig(id, TIM_IT_CC2, ENABLE);
                    break;
                case 3:
                    if (timer > timer_5 && timer != timer_8)
                        return;
                    TIM_ClearITPendingBit(id, TIM_IT_CC3);
                    TIM_ITConfig(id, TIM_IT_CC3, ENABLE);
                    break;
                case 4:
                    if (timer > timer_5 && timer != timer_8)
                        return;
                    TIM_ClearITPendingBit(id, TIM_IT_CC4);
                    TIM_ITConfig(id, TIM_IT_CC4, ENABLE);
                    break;
            }
    }

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = irq_timer_channels[(timer - 1) * 5 + event];
    nvic.NVIC_IRQChannelPreemptionPriority = 0x01;
    nvic.NVIC_IRQChannelSubPriority = 0x01;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    set_timer_callback(timer, event, cb);
}

void timer_irq_deinit(timer_t timer, irq_timer_event_t event){
    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = irq_timer_channels[(timer - 1) * 5 + event];
    nvic.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&nvic);
}

//input capture
