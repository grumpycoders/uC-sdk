#include <reent.h>
#include <malloc.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "osdebug.h"

static xSemaphoreHandle malloc_sem = NULL;

__attribute__((constructor)) static void malloc_init() {
    malloc_sem = xSemaphoreCreateMutex();
}

void * malloc(size_t size) {
    void * ptr;
//    DBGOUT("malloc(%i)\r\n", size);
    
    if (malloc_sem)
        xSemaphoreTake(malloc_sem, portMAX_DELAY);
    ptr =_malloc_r(_impure_ptr, size);
    if (malloc_sem)
        xSemaphoreGive(malloc_sem);
    return ptr;
}
