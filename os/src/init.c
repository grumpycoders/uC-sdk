#include <stdlib.h>
#include <BoardConsole.h>

extern void __libc_init_array();
extern void __libc_fini_array();
extern void __main();
extern int main(int, char **, char **);
extern void BoardEarlyInit();
extern void BoardLateInit();
extern void BoardExceptionHandler(int);
extern void BoardShutdown();

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
    __libc_init_array();
    BoardLateInit();
    atexit(__libc_fini_array);
    exit(main(0, NULL, NULL));
}
