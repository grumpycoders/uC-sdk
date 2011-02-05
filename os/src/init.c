#include <stdlib.h>
#include <stdio.h>
#include <BoardConsole.h>

void __libc_init_array();
void __libc_fini_array();
int main(int, char **, char **);
void BoardEarlyInit();
void BoardLateInit();
void BoardExceptionHandler(int);
void BoardShutdown();

void _exit(int return_code) __attribute__((noreturn));
void _exit(int return_code) {
    if (return_code)
        BoardExceptionHandler(return_code);
    BoardShutdown();
    while(1);
}

void _start() {
    BoardEarlyInit();
    BoardConsoleInit();
    BoardConsolePuts("uC-sdk - booting.");
    __sinit(_impure_ptr);
    __libc_init_array();
    BoardLateInit();
    atexit(__libc_fini_array);
    exit(main(0, NULL, NULL));
}

void startup_memcpy(void * dest, const void * src, size_t n) {
    memcpy(dest, src, n);
}

void startup_memset(void * dest, int c, size_t n) {
    memset(dest, c, n);
}
