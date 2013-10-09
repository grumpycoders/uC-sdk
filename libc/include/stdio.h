#ifndef __STDIO_H__
#define __STDIO_H__

#include <reent.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <malloc.h>

static const int EOF = -1;

struct _FILE {
    int fd;
    int got_eof;
};

typedef struct _FILE FILE;
extern FILE * stdin, * stdout, * stderr;

int vdprintf(int fd, const char * format, va_list ap);
int vsprintf(char * str, const char * format, va_list ap);
int vsnprintf(char * str, size_t size, const char * format, va_list ap);
int vasprintf(char ** strp, const char * format, va_list ap);
int vxprintf(void (*func)(const char *, int, void *), void * arg, const char * format, va_list ap);
static inline int vfprintf(FILE * stream, const char * format, va_list ap) { return vdprintf(stream->fd, format, ap); }
static inline int vprintf(const char * format, va_list ap) { return vfprintf(stdout, format, ap); }

static inline int dprintf(int fd, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vdprintf(fd, format, ap); va_end(ap); return r; }
static inline int sprintf(char * str, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vsprintf(str, format, ap); va_end(ap); return r; }
static inline int snprintf(char * str, size_t size, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vsnprintf(str, size, format, ap); va_end(ap); return r; }
static inline int asprintf(char ** strp, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vasprintf(strp, format, ap); va_end(ap); return r; }
static inline int xprintf(void (*func)(const char *, int, void *), void * arg, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vxprintf(func, arg, format, ap); va_end(ap); return r; }
static inline int fprintf(FILE * stream, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vfprintf(stream, format, ap); va_end(ap); return r; }
static inline int printf(const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vprintf(format, ap); va_end(ap); return r; }

int vdscanf(int fd, const char * format, va_list ap);
int vsscanf(const char * str, const char * format, va_list ap);
int vxscanf(int (*xgetc)(void *), void (*xungetc)(void *, int), void * opaque, const char * format, va_list args);
static inline int vfscanf(FILE * stream, const char * format, va_list ap) { return vdscanf(stream->fd, format, ap); }
static inline int vscanf(const char * format, va_list ap) { return vfscanf(stdin, format, ap); }

static inline int dscanf(int fd, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vdscanf(fd, format, ap); va_end(ap); return r; }
static inline int sscanf(const char * str, const char * format, ...)  { va_list ap; int r; va_start(ap, format); r = vsscanf(str, format, ap); va_end(ap); return r; }
static inline int xscanf(int (*xgetc)(void *), void (*xungetc)(void *, int), void * opaque, const char *format, ...) { va_list ap; int r; va_start(ap, format); r = vxscanf(xgetc, xungetc, opaque, format, ap); va_end(ap); return r; }
static inline int fscanf(FILE * stream, const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vfscanf(stream, format, ap); va_end(ap); return r; }
static inline int scanf(const char * format, ...) { va_list ap; int r; va_start(ap, format); r = vscanf(format, ap); va_end(ap); return r; }



void __sinit(struct _reent *);

// We don't even buffer, so...
static inline int fflush(FILE *stream) { return 0; }

// hopefully, since all of the mode crap is static, gcc will optimize most of it away.
static inline FILE * fopen(const char * fname, const char * mode) {
    FILE * r = NULL;
    int flags = 0, plus = 0, append = 0, fd;
    if (!mode || !mode[0]) {
        set_errno(EINVAL);
        return NULL;
    }
    
    if (mode[1] == 'b') {
        plus = mode[2] == '+';
    } else if (mode[1]) {
        if (mode[1] != '+') {
            set_errno(EINVAL);
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
        set_errno(EINVAL);
        return NULL;
    }
    
    fd = open(fname, flags);
    
    if (fd >= 0) {
        r = (FILE *) malloc(sizeof(FILE));
        r->fd = fd;
        r->got_eof = 0;
    }
    
    if (append)
        lseek(r->fd, 0, SEEK_END);
    
    return r;
}

static inline int fclose(FILE * stream) {
    int fd;
    
    if (!stream) {
        set_errno(EINVAL);
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
    size_t r;
    
    if (!stream) {
        set_errno(EINVAL);
        return -1;
    }
    
    if (size == 1) {
        r = read(stream->fd, ptr, nmemb);
        if (r == 0)
            stream->got_eof = 1;
        return r;
    }
    
    if (nmemb == 1) {
        r = read(stream->fd, ptr, size) == size ? 1 : 0;
        if (r == 0)
            stream->got_eof = 1;
        return r;
    }
    
    for (i = 0; i < nmemb; i++) {
        if (read(stream->fd, ptr + size * i, size) != size) {
            stream->got_eof = 1;
            return i;
        }
    }
    
    return nmemb;
}

static inline size_t fwrite(const void * _ptr, size_t size, size_t nmemb, FILE * stream) {
    int i;
    const uint8_t * ptr = (const uint8_t *) _ptr;
    
    if (!stream) {
        set_errno(EINVAL);
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

static inline int fgetc(FILE * stream) {
    uint8_t v;

    if (!stream) {
        set_errno(EINVAL);
        return -1;
    }
    
    if (read(stream->fd, &v, 1) != 1) {
        stream->got_eof = 1;
        return EOF;
    }
    return v;
}

static inline int fseek(FILE * stream, off_t offset, int wheel) {
    int r;
    if (!stream) {
        set_errno(EINVAL);
        return -1;
    }
    
    r = lseek(stream->fd, offset, wheel) != -1 ? 0 : -1;
    if (!r)
        stream->got_eof = 0;
    return r;
}

static inline char * fgets(char * s, int n, FILE * stream) {
    int r, fd;
    char c, * copy = s;
    if (!stream) {
        set_errno(EINVAL);
        return NULL;
    }
    
    fd = stream->fd;
    
    while (--n) {
        r = read(fd, &c, 1);
        switch (r) {
        case 0:
            stream->got_eof = 1;
            *s = 0;
            return copy;
        case 1:
            *s++ = c;
            if (c == '\n') {
                *s = 0;
                return copy;
            }
            break;
        case -1:
            return NULL;
            break;
        }
    };
    
    *s = 0;
    return copy;
}

static inline int getc() { return fgetc(stdin); }
static inline off_t ftell(FILE * stream) { return lseek(stream->fd, 0, SEEK_CUR); }
static inline int feof(FILE * stream) { return stream->got_eof; }
static inline int fileno(FILE * stream) { return stream->fd; }
static inline void rewind(FILE * stream) { fseek(stream, 0, SEEK_SET); }

#endif
