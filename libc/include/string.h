#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
#include <stddef.h>
#include <malloc.h>

static inline void * memcpy(void * _s1, const void * _s2, size_t n) {
    uint8_t * s1 = (uint8_t *) _s1;
    const uint8_t * s2 = (uint8_t *) _s2;
    size_t i;
    
    for (i = 0; i < n; i++)
        *s1++ = *s2++;
    
    return _s1;
}

static inline void * memmove(void * _s1, const void * _s2, size_t n) {
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

static inline const void * memchr(const void * _s, int c, size_t n) {
    const uint8_t * s = (uint8_t *) _s;
    size_t i;
    
    for (i = 0; i < n; i++, s++)
        if (*s == c)
            return s;
    
    return NULL;
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

static inline const char * strrchr(const char * s, char c) {
    const char * r = NULL;
    
    while (*s) {
        if (*s == c)
            r = s;
        s++;
    }
    
    return r;
}

static inline size_t strlen(const char * s) {
    size_t r = 0;
    
    while (*s++)
        r++;
    
    return r;
}

static inline char * strncat(char * s1, const char * s2, size_t n) {
    char * r = s1;
    
    while (*s1)
        s1++;
    strncpy(s1, s2, n);
    
    return r;
}

static inline int strcmp(const char * s1, const char * s2) {
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

static inline int strncmp(const char * s1, const char * s2, size_t n) {
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

static inline char * strdup(const char * s) {
    return strcpy((char *) malloc(strlen(s) + 1), s);
}

static inline const char * strstr(const char * s1, const char * s2) {
    size_t l = strlen(s2);
    
    while (*s1) {
        if (!strncmp(s1, s2, l))
            return s1;
        s1++;
    }
}

#endif
