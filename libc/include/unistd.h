#ifndef __UNISTD_H__
#define __UNISTD_H__

#include <reent.h>
#include <stddef.h>
#include <stdint.h>

typedef int32_t ssize_t;
typedef int32_t off_t;

enum open_types_t {
    O_RDONLY = 0,
    O_WRONLY = 1,
    O_RDWR = 2,
    O_CREAT = 4,
    O_TRUNC = 8,
    O_APPEND = 16,
};

enum seek_wheels_t {
    SEEK_SET = 0,
    SEEK_CUR = 1,
    SEEK_END = 2,
};

int open(const char *pathname, int flags);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t seek, int wheel);

#endif
