#pragma once

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>

static __inline__ void * memcpy(void * _s1, const void * _s2, size_t n) {
    uint8_t * s1 = (uint8_t *) _s1;
    const uint8_t * s2 = (uint8_t *) _s2;
    size_t i;
    
    for (i = 0; i < n; i++)
        *s1++ = *s2++;
    
    return _s1;
}

static __inline__ void * memmove(void * _s1, const void * _s2, size_t n) {
    uint8_t * s1 = (uint8_t *) _s1;
    const uint8_t * s2 = (uint8_t *) _s2;
    size_t i;
    
    if (s1 < s2) {
        for (i = 0; i < n; i++)
            *s1++ = *s2++;
    } else if (s1 > s2) {
        s1 += n;
        s2 += n;
        for (i = 0; i < n; i++)
            *--s1 = *--s2;
    }
    
    return _s1;
}

static __inline__ int memcmp(const void * _s1, const void * _s2, size_t n) {
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

static __inline__ void * memset(void * _s, int c, size_t n) {
    uint8_t * s = (uint8_t *) _s;
    size_t i;
    
    for (i = 0; i < n; i++)
        *s++ = (uint8_t) c;
    
    return _s;
}

static __inline__ const void * memchr(const void * _s, int c, size_t n) {
    const uint8_t * s = (uint8_t *) _s;
    size_t i;
    
    for (i = 0; i < n; i++, s++)
        if (*s == c)
            return s;
    
    return NULL;
}

static __inline__ char * strcat(char * s1, const char * s2) {
    char * r = s1;
    
    while (*s1)
        s1++;
    
    while (*s2)
        *s1++ = *s2++;
    
    *s1 = 0;
    
    return r;
}

static __inline__ char * strcpy(char * s1, const char * s2) {
    char * r = s1;
    
    while ((*s1++ = *s2++));
    
    return r;
}

static __inline__ char * strncpy(char * s1, const char * s2, size_t n) {
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

static __inline__ char * strchr(const char * s, char c) {
    while (*s) {
        if (*s == c)
            return (char *) s;
        s++;
    }
    
    return NULL;
}

static __inline__ char * strrchr(const char * s, char c) {
    const char * r = NULL;
    
    while (*s) {
        if (*s == c)
            r = s;
        s++;
    }
    
    return (char *) r;
}

static __inline__ size_t strlen(const char * s) {
    size_t r = 0;
    
    while (*s++)
        r++;
    
    return r;
}

static __inline__ char * strncat(char * s1, const char * s2, size_t n) {
    char * r = s1;
    
    while (*s1)
        s1++;
    strncpy(s1, s2, n);
    
    return r;
}

static __inline__ int strcmp(const char * s1, const char * s2) {
    while (*s1 && *s2) {
        if (!*s1) {
            return -1;
        } else if (!*s2) {
            return 1;
        } else if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        }
        s1++;
        s2++;
    }
    
    return 0;
}

static __inline__ int strncmp(const char * s1, const char * s2, size_t n) {
    while (*s1 && *s2 && n) {
        if (!*s1) {
            return -1;
        } else if (!*s2) {
            return 1;
        } else if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        }
        s1++;
        s2++;
        n--;
    }
    
    return 0;
}

static __inline__ char * strdup(const char * s) {
    return strcpy((char *) malloc(strlen(s) + 1), s);
}

static __inline__ char * strstr(const char * s1, const char * s2) {
    size_t l = strlen(s2);
    
    while (*s1) {
        if (!strncmp(s1, s2, l))
            return (char *) s1;
        s1++;
    }
    
    return NULL;
}

static __inline__ char * strerror(int errnum) { return errnum ? "Not compiled with error strings" : "No error"; }
