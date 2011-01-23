#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "LPC17xx.h"

#define configUSE_PREEMPTION        1
#define configUSE_IDLE_HOOK         1
#define configMAX_PRIORITIES        ( ( unsigned portBASE_TYPE ) 5 )
#define configUSE_TICK_HOOK         1
#define configCPU_CLOCK_HZ          ( SystemCoreClock )
#define configTICK_RATE_HZ          ( ( portTickType ) 1000 )
#define configMINIMAL_STACK_SIZE    ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE       ( ( size_t ) ( 19 * 1024 ) )
#define configMAX_TASK_NAME_LEN     ( 12 )
#define configUSE_TRACE_FACILITY    0
#define configUSE_16_BIT_TICKS      0
#define configIDLE_SHOULD_YIELD     1
#define configUSE_CO_ROUTINES       0
#define configUSE_MUTEXES           1
#define configUSE_NEWLIB_REENTRANT  1

#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

#define configUSE_COUNTING_SEMAPHORES   0
#define configUSE_ALTERNATIVE_API       0
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configUSE_RECURSIVE_MUTEXES     1
#define configQUEUE_REGISTRY_SIZE       10
#define configGENERATE_RUN_TIME_STATS   1

#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_uxTaskGetStackHighWaterMark 1

/* Use the system definition, if there is one */
#ifdef __NVIC_PRIO_BITS
    #define configPRIO_BITS       __NVIC_PRIO_BITS
#else
    #define configPRIO_BITS       5        /* 32 priority levels */
#endif

/* The lowest priority. */
#define configKERNEL_INTERRUPT_PRIORITY     ( 31 << (8 - configPRIO_BITS) )
/* Priority 5, or 160 as only the top three bits are implemented. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( 5 << (8 - configPRIO_BITS) )

/* Priorities passed to NVIC_SetPriority() do not require shifting as the
function does the shifting itself.  Note these priorities need to be equal to
or lower than configMAX_SYSCALL_INTERRUPT_PRIORITY - therefore the numeric
value needs to be equal to or greater than 5 (on the Cortex M3 the lower the
numeric value the higher the interrupt priority). */
#define configEMAC_INTERRUPT_PRIORITY       5
#define configUSB_INTERRUPT_PRIORITY        6

/*-----------------------------------------------------------
 * Macros required to setup the timer for the run time stats.
 *-----------------------------------------------------------*/
extern void vConfigureTimerForRunTimeStats( void );
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE() (LPC_TIM0->TC)

#endif /* FREERTOS_CONFIG_H */
