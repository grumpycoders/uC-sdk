#include <stdlib.h>
#include <BoardConsole.h>

extern void __libc_init_array() __attribute__((weak));
extern void __libc_fini_array() __attribute__((weak));
extern void __main() __attribute__ ((weak));
extern int main(int, char **, char **);
extern void BoardEarlyInit() __attribute__((weak));
extern void BoardLateInit() __attribute__((weak));
extern void BoardExceptionHandler(int) __attribute__((weak));
extern void BoardShutdown() __attribute__((weak));

void _exit(int return_code) __attribute__((noreturn));
void _exit(int return_code) {
    if (return_code && BoardExceptionHandler)
        BoardExceptionHandler(return_code);
    if (BoardShutdown)
        BoardShutdown();
    while(1);
}

void _start() {
    if (BoardEarlyInit)
        BoardEarlyInit();
    BoardConsoleInit();
    BoardConsolePuts("uC-sdk - booting.");    
    if (__libc_init_array)
        __libc_init_array();
    
    if (__main)
        __main();
    
    if (BoardLateInit)
        BoardLateInit();

    if (__libc_fini_array)
        atexit(__libc_fini_array);
    
    exit(main(0, NULL, NULL));
}
