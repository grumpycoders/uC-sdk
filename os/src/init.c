#include <stdlib.h>

extern void __libc_init_array(void) __attribute__ ((weak));
extern void __libc_fini_array(void) __attribute__ ((weak));
extern void exit(int) __attribute__ ((noreturn, weak));
extern void __main() __attribute__ ((weak));
extern int main(int, char **, char **);

void _start() {
    if (__libc_init_array)
        __libc_init_array();
    
    if (__main)
        __main();
    
    int return_code = main(0, NULL, NULL);
    
    if (__libc_fini_array)
        __libc_fini_array();
    
    if (exit)
        exit(return_code);
}
