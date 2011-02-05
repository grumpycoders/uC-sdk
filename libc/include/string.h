#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
#include <stddef.h>

static inline void * memcpy(void * _s1, const void * _s2, size_t n) {
    uint8_t * s1 = (uint8_t *) _s1;
    const uint8_t * s2 = (uint8_t *) _s2;
    size_t i;
    
    for (i = 0; i < n; i++)
        *s1++ = *s2++;
    
    return _s1;
}

static inline int memcmp(const void * _s1, const void * _s2, size_t n) {
    uint8_t * s1 = (uint8_t *) _s1;
    const uint8_t * s2 = (uint8_t *) _s2;
    size_t i;
    
    for (i = 0; i < n; i++, s1++, s2++) {
        if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        }
    }
    
    return 0;
}

static inline void * memset(void * _s, int c, size_t n) {
    uint8_t * s = (uint8_t *) _s;
    size_t i;
    
    for (i = 0; i < n; i++)
        *s++ = (uint8_t) c;
    
    return _s;
}

static inline char * strcat(char * s1, const char * s2) {
    char * r = s1;
    
    while (*s1)
        s1++;
    
    while (*s2)
        *s1++ = *s2++;
    
    *s1 = 0;
    
    return r;
}

static inline char * strcpy(char * s1, const char * s2) {
    char * r = s1;
    
    while ((*s1++ = *s2++));
    
    return r;
}

static inline char * strncpy(char * s1, const char * s2, size_t n) {
    char * r = s1;
    size_t i;
    
    for (i = 0; i < n; i++) {
        if (*s2) {
            *s1++ = *s2++;
        } else {
            *s1++ = 0;
        }
    }
    
    return r;
}

static inline const char * strchr(const char * s, char c) {
    while (*s) {
        if (*s == c)
            return s;
        s++;
    }
    return NULL;
}

static inline size_t strlen(const char * s) {
    size_t r = 0;
    
    while (*s++)
        r++;
    
    return r;
}

#endif
