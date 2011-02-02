#include <FreeRTOS.h>
#include <task.h>
#include "osdebug.h"

void vConfigureTimerForRunTimeStats() {
    DBGOUT("vConfigureTimerForRunTimeStats()\r\n");
}

void vApplicationTickHook() {
//    DBGOUT("vApplicationTickHook()\r\n");
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName) {
    DBGOUT("vApplicationStackOverflowHook(%p, %s)\r\n", pxTask, pcTaskName);
}

void vApplicationIdleHook() {
}
