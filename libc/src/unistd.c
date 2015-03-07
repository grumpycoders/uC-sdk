#include "stdio.h"
#include "reent.h"
#include "errno.h"
#include <fio.h>
#include <filesystem.h>

int open(const char * path, int flags) {
    int r;

    r = fs_open(path, flags, 0755);

    if (r >= 0)
        return r;

    set_errno(EACCES);
    return -1;
}

int close(int fd) {
    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }

    return fio_close(fd);
}

ssize_t read(int fd, void * buf, size_t size) {
    ssize_t r;

    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }

    r = fio_read(fd, buf, size);

    if (r < 0) {
        set_errno(EINVAL);
        return -1;
    }

    return r;
}

ssize_t write(int fd, const void * buf, size_t size) {
    ssize_t r;

    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }

    r = fio_write(fd, buf, size);

    if (r < 0) {
        set_errno(EINVAL);
        return -1;
    }

    return r;
}

off_t lseek(int fd, off_t seek, int wheel) {
    off_t r;

    if ((wheel != SEEK_SET) && (wheel != SEEK_CUR) && (wheel != SEEK_END)) {
        set_errno(EINVAL);
        return -1;
    }

    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }

    r = fio_seek(fd, seek, wheel);

    if (r < 0)
        set_errno(EINVAL);

    return r;
}
