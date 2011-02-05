#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <reent.h>
#include <malloc.h>

typedef void (*atexit_func_t)(void);

void exit(int status) __attribute__((noreturn));
int atexit(atexit_func_t);

void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

#endif
