#include <reent.h>
#include <errno.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <mpu_wrappers.h>

#include "osdebug.h"

extern uintptr_t __heap_start;
extern uintptr_t __stack_start;

static void * heap_end = NULL;

void * sbrk(ptrdiff_t incr) {
    void *prev_heap_end, *next_heap_end, *ret;
    void *stack_min = (void *) &__stack_start;

    prev_heap_end = heap_end ? heap_end : (void *) &__heap_start;

    /* Align to always be on 8-byte boundaries */
    next_heap_end = (void *)((((uintptr_t)prev_heap_end + incr) + 7) & ~7);

    /* Check if this allocation would exceed the end of the ram - would probably get into the stack first however */
    if (next_heap_end > stack_min) {
        set_errno(ENOMEM);
        ret = NULL;
    } else {
        heap_end = next_heap_end;
        ret = (void *)prev_heap_end;
    }

    return ret;
}
