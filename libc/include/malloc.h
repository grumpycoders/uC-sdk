#pragma once

#include <decl.h>
#include <reent.h>
#include <stddef.h>
#include <stdint.h>

BEGIN_DECL

typedef void * (*malloc_t)(size_t size);
typedef void (*free_t)(void * ptr);
typedef void * (*realloc_t)(void * ptr, size_t size);

void * base_malloc(size_t size);
void base_free(void * ptr);
void * base_realloc(void * ptr, size_t size);

void * malloc(size_t size);
void free(void * ptr);
void * realloc(void * ptr, size_t size);

extern malloc_t malloc_ptr;
extern free_t free_ptr;
extern realloc_t realloc_ptr;

static __inline__ void * calloc(size_t nmemb, size_t size) {
    uint8_t * r = (uint8_t *)malloc(nmemb * size);
    size_t i;
    for (i = 0; i < (size * nmemb); i++)
        r[i] = 0;
    return (void *)r;
}

END_DECL
