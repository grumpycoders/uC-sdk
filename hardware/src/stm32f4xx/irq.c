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
