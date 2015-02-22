#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <BoardConsole.h>
#include <stdio.h>
#include <fio.h>
#include <romfs.h>
#include <semifs.h>
#include <malloc_wrapper.h>
#include <devfs.h>

static const char msg[] = "Hello world - from fwrite!\r\n";

extern uint8_t romfs[];

int main() {
    init_malloc_wrapper();
    FILE * f1, * f2;
    char buf[32];
    int c;
    register_devfs();
    register_romfs("romfs", romfs);
    printf("Hello world - from stdio!\r\n");
    fflush(stdout);
    f1 = fopen("/dev/stdout", "w");
    fprintf(stderr, "f1 = %p\r\n", f1);
    fwrite(msg, 1, sizeof(msg), f1);
    f2 = fopen("/romfs/test.txt", "r");
    c = fread(buf, 1, 32, f2);
    fwrite(buf, 1, c, f1);
    fflush(f1);
    fclose(f1);
    fclose(f2);
    return 0;
}
