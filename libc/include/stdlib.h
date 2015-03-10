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

extern unsigned int _seed;

int rand_r(unsigned int * seed); 

static __inline__ int rand(void) { return rand_r(&_seed); }
static __inline__ void srand(unsigned int seed) { _seed = seed * 3835238167UL; }

END_DECL

static __inline__ double atof(const char * str) { double r = 0; sscanf(str, "%lf", &r); return r; }
static __inline__ int atoi(const char * str) { int i; sscanf(str, "%i", &i); return i; }
static __inline__ int atol(const char * str) { long l; sscanf(str, "%li", &l); return l; }
