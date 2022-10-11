#include <string.h>

void * memcpy(void * _s1, const void * _s2, size_t n) {
    uint8_t * s1 = (uint8_t *) _s1;
    const uint8_t * s2 = (uint8_t *) _s2;
    size_t i;

    for (i = 0; i < n; i++)
        *s1++ = *s2++;

    return _s1;
}

void * memmove(void * _s1, const void * _s2, size_t n) {
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

int memcmp(const void * _s1, const void * _s2, size_t n) {
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

void * memset(void * _s, int c, size_t n) {
    uint8_t * s = (uint8_t *) _s;
    size_t i;

    for (i = 0; i < n; i++)
        *s++ = (uint8_t) c;

    return _s;
}
