#include <reent.h>
#include <errno.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "mpu_wrappers.h"

// Mostly stolen from mbed-freertos

extern unsigned int __cs3_heap_start, __cs3_heap_end;

/* Low-level bulk RAM allocator -- used by Newlib's Malloc */
void *heap_end = NULL;
PRIVILEGED_FUNCTION void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    void *prev_heap_end, *next_heap_end, *ret;

    taskENTER_CRITICAL();
    {
        /* Initialize on first call */
        if (heap_end == NULL)
        {
            heap_end = (void *)&__cs3_heap_start;
        }

        prev_heap_end = heap_end;

        /* Align to always be on 8-byte boundaries */
        next_heap_end = (void *)((((unsigned int)heap_end + incr) + 7) & ~7);

        /* Check if this allocation would exceed the end of the ram - would probably get into the stack first however */
        if (next_heap_end > (void *)&__cs3_heap_end)
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

