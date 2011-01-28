#include <reent.h>
#include <errno.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <mpu_wrappers.h>

#include "osdebug.h"

// Mostly stolen from mbed-freertos

extern uintptr_t __heap_start;
extern uintptr_t __stack_start;

/* Low-level bulk RAM allocator -- used by Newlib's Malloc */
static void *heap_end = (void *) &__heap_start;

#define USE_SBRK_MUTEX 0

#if USE_SBRK_MUTEX
static xSemaphoreHandle sbrk_sem;

__attribute__((constructor)) static void sbrk_init() {
    DBGOUT("Creating Mutex...");
    sbrk_sem = xSemaphoreCreateMutex();
}
#endif

void *_sbrk_r(struct _reent *ptr, ptrdiff_t incr)
{
    void *prev_heap_end, *next_heap_end, *ret;
    void *stack_min = (void *) &__stack_start;

#if USE_SBRK_MUTEX
    xSemaphoreTake(sbrk_sem, portMAX_DELAY);
#endif

    prev_heap_end = heap_end;

    /* Align to always be on 8-byte boundaries */
    next_heap_end = (void *)((((uintptr_t)heap_end + incr) + 7) & ~7);

    /* Check if this allocation would exceed the end of the ram - would probably get into the stack first however */
    if (next_heap_end > stack_min) {
        ptr->_errno = ENOMEM;
        ret = NULL;
    } else {
        heap_end = next_heap_end;
        ret = (void *)prev_heap_end;
    }

#if USE_SBRK_MUTEX
    xSemaphoreGive(sbrk_sem);
#endif

    return ret;
}

