#include <reent.h>
#include <errno.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "mpu_wrappers.h"

#include <osdebug.h>

// Mostly stolen from mbed-freertos

extern uintptr_t __heap_start, __heap_end;
//extern uintptr_t __stack_start;

/* Low-level bulk RAM allocator -- used by Newlib's Malloc */
static void *heap_end = NULL;

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    void *prev_heap_end, *next_heap_end, *ret;
//    void *stack_min = (void *)(__stack_start ? __stack_start : __heap_end);
    void *stack_min = (void *)__heap_end;
    
    DBGOUT("_sbrk_r(%p, %u)\r\n", ptr, incr);

    taskENTER_CRITICAL();
    {
        /* Initialize on first call */
        if (heap_end == NULL)
        {
            heap_end = (void *)&__heap_start;
        }

        prev_heap_end = heap_end;

        /* Align to always be on 8-byte boundaries */
        next_heap_end = (void *)((((uintptr_t)heap_end + incr) + 7) & ~7);

        /* Check if this allocation would exceed the end of the ram - would probably get into the stack first however */
        if (next_heap_end > stack_min)
        {
            ptr->_errno = ENOMEM;
            ret = NULL;
        }
        else
        {
            heap_end = next_heap_end;
            ret = (void *)prev_heap_end;
        }
    }
    taskEXIT_CRITICAL();
    return ret;
}

