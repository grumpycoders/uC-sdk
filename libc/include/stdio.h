#ifndef __STDIO_H__
#define __STDIO_H__

#include <reent.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <malloc.h>

struct _FILE {
    int fd;
};

typedef struct _FILE FILE;

int printf(const char * format, ...);
int fprintf(FILE *stream, const char * format, ...);
int sprintf(char * str, const char * format, ...);
int snprintf(char * str, size_t size, const char * format, ...);
int asprintf(char ** strp, const char * format, ...);

int vprintf(const char * format, va_list ap);
int vfprintf(FILE *stream, const char * format, va_list ap);
int vsprintf(char * str, const char * format, va_list ap);
int vsnprintf(char * str, size_t size, const char * format, va_list ap);
int vasprintf(char ** strp, const char * format, va_list ap);

void __sinit(struct _reent *);

// We don't even buffer, so...
static inline int fflush(FILE *stream) { return 0; }

// hopefully, since all of the mode crap is static, gcc will optimize most of it away.
static inline FILE * fopen(const char * fname, const char * mode) {
    FILE * r = NULL;
    int flags = 0, plus = 0, append = 0, fd;
    if (!mode || !mode[0]) {
        _impure_ptr->_errno = EINVAL;
        return NULL;
    }
    
    if (mode[1] == 'b') {
        plus = mode[2] == '+';
    } else if (mode[1]) {
        if (mode[1] != '+') {
            _impure_ptr->_errno = EINVAL;
            return NULL;
        }
        plus = 1;
    }
    
    switch (mode[0]) {
    case 'r':
        if (plus) {
            flags = O_RDWR;
        } else {
            flags = O_RDONLY;
        }
        break;
    case 'w':
        if (plus) {
            flags = O_RDWR | O_CREAT | O_TRUNC;
        } else {
            flags = O_WRONLY | O_CREAT | O_TRUNC;
        }
        break;
    case 'a':
        append = 1;
        if (plus) { // won't be properly supported
            flags = O_RDWR | O_CREAT;
        } else {
            flags = O_WRONLY | O_CREAT;
        }
        break;
    default:
        _impure_ptr->_errno = EINVAL;
        return NULL;
    }
    
    fd = open(fname, flags);
    
    if (fd >= 0) {
        r = (FILE *) malloc(sizeof(FILE));
        r->fd = fd;
    }
    
    return r;
}

static inline int fclose(FILE * stream) {
    int fd;
    
    if (!stream) {
        _impure_ptr->_errno = EINVAL;
        return -1;
    }
    
    fd = stream->fd;
    free(stream);
    return close(fd);
}

// Again, the compiler should do the right thing, and optimize depending on the values of size and nmemb.
// Luckily, we always will get into the short cases.
static inline size_t fread(void * _ptr, size_t size, size_t nmemb, FILE * stream) {
    int i;
    uint8_t * ptr = (uint8_t *) _ptr;
    
    if (!stream) {
        _impure_ptr->_errno = EINVAL;
        return -1;
    }
    
    if (size == 1)
        return read(stream->fd, ptr, nmemb);
    
    if (nmemb == 1)
        return read(stream->fd, ptr, size) == size ? 1 : 0;
    
    for (i = 0; i < nmemb; i++) {
        if (read(stream->fd, ptr + size * i, size) != size)
            return i;
    }
    
    return nmemb;
}

static inline size_t fwrite(const void * _ptr, size_t size, size_t nmemb, FILE * stream) {
    int i;
    const uint8_t * ptr = (const uint8_t *) _ptr;
    
    if (!stream) {
        _impure_ptr->_errno = EINVAL;
        return -1;
    }
    
    if (size == 1)
        return write(stream->fd, ptr, nmemb);
    
    if (nmemb == 1)
        return write(stream->fd, ptr, size) == size ? 1 : 0;
    
    for (i = 0; i < nmemb; i++) {
        if (write(stream->fd, ptr + size * i, size) != size)
            return i;
    }
    
    return nmemb;
}

extern FILE * stdin, * stdout, * stderr;

#endif
