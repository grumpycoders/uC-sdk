#include "stdio.h"

struct opaque_t {
    union {
        const char * p;
        int fd;
    };
    uint8_t bsize;
    char backbuffer[3];
};

static int str_getc(void * _opaque) {
    struct opaque_t * opaque = (struct opaque_t *) _opaque;
    int r;

    if (opaque->bsize)
        return opaque->backbuffer[--(opaque->bsize)];
    
    r = *(opaque->p++);
    
    return r ? r : EOF;
}

static int file_getc(void * _opaque) {
    struct opaque_t * opaque = (struct opaque_t *) _opaque;
    uint8_t v;

    if (opaque->bsize)
        return opaque->backbuffer[--(opaque->bsize)];
    
    if (read(opaque->fd, &v, 1) != 1)
        return EOF;
    return v;
}

static void scanf_ungetc(void * _opaque, int c) {
    struct opaque_t * opaque = (struct opaque_t *) _opaque;
    
    if (opaque->bsize < sizeof(opaque->backbuffer))
        opaque->backbuffer[opaque->bsize++] = c;
}

int vsscanf(const char * str, const char * format, va_list ap) {
    struct opaque_t opaque;
    opaque.p = str;
    opaque.bsize = 0;
    return vxscanf(str_getc, scanf_ungetc, &opaque, format, ap);
}

int vdscanf(int fd, const char * format, va_list ap) {
    struct opaque_t opaque;
    opaque.fd = fd;
    opaque.bsize = 0;
    return vxscanf(file_getc, scanf_ungetc, &opaque, format, ap);
}
