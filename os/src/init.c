#include <stdlib.h>
#include <string.h>
#include <BoardConsole.h>

int main(int, char **, char **);

void BoardEarlyInit();
void BoardInit();
void BoardLateInit();

void BoardExceptionHandler(int);
void BoardShutdown();

void cpu_early_init();
void cpu_init();
void cpu_late_init();

void libc_init();

void _exit(int return_code) __attribute__((noreturn));
void _exit(int return_code) {
    if (return_code)
        BoardExceptionHandler(return_code);
    BoardShutdown();
    while(1);
}

void _ucsdk_start() {
    BoardEarlyInit();
    cpu_early_init();

    BoardConsoleInit();
    BoardConsolePuts("\n\n================\nuC-sdk - booting\n================\n");

    libc_init();

    BoardInit();
    cpu_init();

    BoardLateInit();
    cpu_late_init();

    exit(main(0, NULL, NULL));
}

void startup_memcpy(void * dest, const void * src, size_t n) {
    memcpy(dest, src, n);
}

void startup_memset(void * dest, int c, size_t n) {
    memset(dest, c, n);
}
