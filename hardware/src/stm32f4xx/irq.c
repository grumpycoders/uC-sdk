#include "irq.h"

#include <stm32f4xx.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stddef.h>

static void (*systick_callback)() = NULL;
/*
void SysTick_Handler(void){
    if (systick_callback)
        systick_callback();
}
*/
void set_timer(uint32_t ms, void (*cb) ()){
    systick_callback = cb;
    SysTick_Config(ms * SystemCoreClock/1000);
}

void unset_timer(){
    systick_callback = NULL;
    SysTick_Config(0xffffffff);
}

uint8_t irq_gpio_channels[] = {
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
static void (*irq_exti_callback[IRQ_EXTI_MAX])();

static void call_exti_callback(int index) {
    if (irq_exti_callback[index]) {
        irq_exti_callback[index]();
    }
}

__attribute__((constructor)) static void irq_init() {
    int i;
    for (i = 0; i < IRQ_EXTI_MAX; i++)
        irq_exti_callback[i] = NULL;
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

void irq_gpio_init(pin_t pin, void (*cb)(), irq_trigger_t tt)
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
    nvic.NVIC_IRQChannel = irq_gpio_channels[pin.pin];
    nvic.NVIC_IRQChannelPreemptionPriority = 0x01;
    nvic.NVIC_IRQChannelSubPriority = 0x01;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    irq_exti_callback[pin.pin] = cb;
}

void irq_gpio_deinit(pin_t pin)
{
    EXTI_InitTypeDef exti;
    exti.EXTI_Line = 1 << pin.pin;
    exti.EXTI_LineCmd = DISABLE;
    EXTI_Init(&exti);

    NVIC_InitTypeDef nvic;
    nvic.NVIC_IRQChannel = irq_gpio_channels[pin.pin];
    nvic.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&nvic);

    irq_exti_callback[pin.pin] = NULL;
}
