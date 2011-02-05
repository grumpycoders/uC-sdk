#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <reent.h>
#include <stddef.h>
#include <string.h>

void * malloc(size_t size);
void free(void *ptr);
void * realloc(void *ptr, size_t size);

static inline void * calloc(size_t nmemb, size_t size) {
    void * r = malloc(nmemb * size);
    memset(r, 0, nmemb * size);
    return r;
}

#endif
