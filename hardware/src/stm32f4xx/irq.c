#include "irq.h"

#include <stm32f4xx.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stddef.h>

#include "hardware.h"

static void (*systick_callback)() = NULL;
/*
void SysTick(void){
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

extern  void * volatile __cs3_interrupt_vector_cortex_m_mutable[];

void *set_irq_handler(uint16_t irq, void * handler) {
    void * ret = NULL;
    do {
        ret = __cs3_interrupt_vector_cortex_m_mutable[irq];
    } while (!__sync_bool_compare_and_swap(__cs3_interrupt_vector_cortex_m_mutable + irq, ret, handler));
    return ret;
}

/*
  //enable interrupts
  NVIC_InitTypeDef nvic;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_X);
  nvic.NVIC_IRQChannel = XXX_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = X;
  nvic.NVIC_IRQChannelSubPriority = X;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);
*/
