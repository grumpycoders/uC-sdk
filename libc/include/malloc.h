#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <reent.h>
#include <stddef.h>
#include <string.h>

typedef void * (*malloc_t)(size_t size);
typedef void (*free_t)(void * ptr);
typedef void * (*realloc_t)(void * ptr, size_t size);

void * base_malloc(size_t size);
void base_free(void * ptr);
void * base_realloc(void * ptr, size_t size);

extern malloc_t malloc;
extern free_t free;
extern realloc_t realloc;

static inline void * calloc(size_t nmemb, size_t size) {
    void * r = malloc(nmemb * size);
    memset(r, 0, nmemb * size);
    return r;
}

#endif
