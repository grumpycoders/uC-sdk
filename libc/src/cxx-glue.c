#include <stdio.h>
#include <stdlib.h>

void _exit(int return_code) __attribute__((noreturn));

__attribute__((weak)) void __cxa_pure_virtual() {
  fprintf(stderr, "pure virtual method called\n");
  _exit(-1);
}

__attribute__((weak)) void * __builtin_new(size_t size) { return malloc(size); }
__attribute__((weak)) void __builtin_delete(void * ptr) { free(ptr); }
__attribute__((weak)) void * _Znwj(unsigned int size) { return malloc(size); }
__attribute__((weak)) void * _Znaj(unsigned int size) { return malloc(size); }
__attribute__((weak)) void _ZdlPv(void * ptr) { free(ptr); }
__attribute__((weak)) void _ZdaPv(void * ptr) { free(ptr); }
