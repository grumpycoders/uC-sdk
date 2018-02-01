#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern void (*__preinit_array_start []) (void) __attribute__((weak));
extern void (*__preinit_array_end []) (void) __attribute__((weak));
extern void (*__init_array_start []) (void) __attribute__((weak));
extern void (*__init_array_end []) (void) __attribute__((weak));
extern void (*__fini_array_start []) (void) __attribute__((weak));
extern void (*__fini_array_end []) (void) __attribute__((weak));

void _init();
void _fini();
void _exit(int return_code) __attribute__((noreturn));


static void __libc_init_array() {
    size_t count, i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i]();

    _init();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i]();
}

static void __libc_fini_array() {
    ssize_t count, i;

    count = __fini_array_end - __fini_array_start;
    for (i = count - 1; i >= 0; i--)
        __fini_array_start[i]();

    _fini();
}

#define MAX_ATEXIT 32

typedef void (*cxa_atexit_t)(void *);
typedef void * cxa_atexit_arg_t;

static volatile uint8_t atexit_count = 0;
static volatile cxa_atexit_t atexit_funcs[MAX_ATEXIT];
static volatile void * atexit_args[MAX_ATEXIT];

int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle) {
    uint8_t pos = __sync_fetch_and_add(&atexit_count, 1);

    if (pos >= MAX_ATEXIT) {
        atexit_count = 32;
        return -1;
    }

    atexit_funcs[pos] = func;
    atexit_args[pos] = arg;
    return 0;
}

/* ARM's eabi version, which swaps arg and func */
int __aeabi_atexit (void * arg, void (*func) (void *), void * dso_handle) {
    return __cxa_atexit(func, arg, dso_handle);
}

int atexit(atexit_func_t func) {
    return __cxa_atexit((cxa_atexit_t) func, NULL, NULL);
}

void exit(int return_code) {
    uint8_t i;

    for (i = 0; i < atexit_count; i++) {
        atexit_funcs[i]((void *)atexit_args[i]);
    }

    __libc_fini_array();
    _exit(return_code);
}

void libc_init() {
    __libc_init_array();
    __sinit(_impure_ptr);
}
