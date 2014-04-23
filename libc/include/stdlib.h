#pragma once

#include <decl.h>
#include <reent.h>
#include <malloc.h>
#include <stdio.h>

typedef void (*atexit_func_t)(void);

BEGIN_DECL

void exit(int status) __attribute__((noreturn));
int atexit(atexit_func_t);

void qsort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *));

END_DECL

static inline double atof(const char * str) { double r = 0; sscanf(str, "%lf", &r); return r; }
static inline int atoi(const char * str) { int i; sscanf(str, "%i", &i); return i; }
static inline int atol(const char * str) { long l; sscanf(str, "%li", &l); return l; }
