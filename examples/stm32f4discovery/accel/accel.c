#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <BoardConsole.h>
#include <stdio.h>
#include <fio.h>
#include <malloc_wrapper.h>

static const char msg[] = "Hello world - from fwrite!\r\n";

extern uint8_t romfs[];

int main() {
    init_malloc_wrapper();

    return 0;
}
