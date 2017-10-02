#include <BoardConsole.h>
#include <string.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <unistd.h>
#include "fio.h"
#include "devfs.h"
#include "filesystem.h"
#include "osdebug.h"
#include "hash-djb2.h"

struct fddef_t {
    fdread_t fdread;
    fdwrite_t fdwrite;
    fdseek_t fdseek;
    fdclose_t fdclose;
    dev_setblock_t setblock;
    void * opaque;
};

static struct fddef_t fio_fds[MAX_FDS];

static ssize_t stdin_read(void * opaque, void * buf, size_t count) {
    return 0;
}

static ssize_t stdout_write(void * opaque, const void * buf, size_t count) {
    int i;
    const char * data = (const char *) buf;

    for (i = 0; i < count; i++)
        BoardConsolePutc(data[i]);

    return count;
}

static xSemaphoreHandle fio_sem = NULL;

__attribute__((constructor)) static void fio_init() {
    memset(fio_fds, 0, sizeof(fio_fds));
    fio_fds[0].fdread = stdin_read;
    fio_fds[1].fdwrite = stdout_write;
    fio_fds[2].fdwrite = stdout_write;
    _uc_sdk_ensure_malloc_exists();
    fio_sem = xSemaphoreCreateMutex();
}

static int fio_is_open_int(int fd) {
    if ((fd < 0) || (fd >= MAX_FDS))
        return 0;
    int r = !((fio_fds[fd].fdread == NULL) &&
              (fio_fds[fd].fdwrite == NULL) &&
              (fio_fds[fd].fdseek == NULL) &&
              (fio_fds[fd].fdclose == NULL) &&
              (fio_fds[fd].opaque == NULL));
    return r;
}

static int fio_findfd() {
    int i;

    for (i = 0; i < MAX_FDS; i++) {
        if (!fio_is_open_int(i))
            return i;
    }

    return -1;
}

int fio_is_open(int fd) {
    int r = 0;
    xSemaphoreTake(fio_sem, portMAX_DELAY);
    r = fio_is_open_int(fd);
    xSemaphoreGive(fio_sem);
    return r;
}

int fio_open(fdread_t fdread, fdwrite_t fdwrite, fdseek_t fdseek, fdclose_t fdclose, void * opaque) {
    int fd;
//    DBGOUT("fio_open(%p, %p, %p, %p, %p)\r\n", fdread, fdwrite, fdseek, fdclose, opaque);
    xSemaphoreTake(fio_sem, portMAX_DELAY);
    fd = fio_findfd();

    if (fd >= 0) {
        fio_fds[fd].fdread = fdread;
        fio_fds[fd].fdwrite = fdwrite;
        fio_fds[fd].fdseek = fdseek;
        fio_fds[fd].fdclose = fdclose;
        fio_fds[fd].setblock = NULL;
        fio_fds[fd].opaque = opaque;
    }
    xSemaphoreGive(fio_sem);

    return fd;
}

int dev_open(fdread_t fdread, fdwrite_t fdwrite, fdseek_t fdseek, fdclose_t fdclose, dev_setblock_t setblock, void * opaque) {
    int fd;
//    DBGOUT("fio_open(%p, %p, %p, %p, %p)\r\n", fdread, fdwrite, fdseek, fdclose, opaque);
    xSemaphoreTake(fio_sem, portMAX_DELAY);
    fd = fio_findfd();

    if (fd >= 0) {
        fio_fds[fd].fdread = fdread;
        fio_fds[fd].fdwrite = fdwrite;
        fio_fds[fd].fdseek = fdseek;
        fio_fds[fd].fdclose = fdclose;
        fio_fds[fd].setblock = setblock;
        fio_fds[fd].opaque = opaque;
    }
    xSemaphoreGive(fio_sem);

    return fd;
}

ssize_t fio_read(int fd, void * buf, size_t count) {
    ssize_t r = 0;
//    DBGOUT("fio_read(%i, %p, %i)\r\n", fd, buf, count);
    if (fio_is_open_int(fd)) {
        if (fio_fds[fd].fdread) {
            r = fio_fds[fd].fdread(fio_fds[fd].opaque, buf, count);
        } else {
            r = -3;
        }
    } else {
        r = -2;
    }
    return r;
}

ssize_t fio_write(int fd, const void * buf, size_t count) {
    ssize_t r = 0;
//    DBGOUT("fio_write(%i, %p, %i)\r\n", fd, buf, count);
    if (fio_is_open_int(fd)) {
        if (fio_fds[fd].fdwrite) {
            r = fio_fds[fd].fdwrite(fio_fds[fd].opaque, buf, count);
        } else {
            r = -3;
        }
    } else {
        r = -2;
    }
    return r;
}

off_t fio_seek(int fd, off_t offset, int whence) {
    off_t r = 0;
//    DBGOUT("fio_seek(%i, %i, %i)\r\n", fd, offset, whence);
    if (fio_is_open_int(fd)) {
        if (fio_fds[fd].fdseek) {
            r = fio_fds[fd].fdseek(fio_fds[fd].opaque, offset, whence);
        } else {
            r = -3;
        }
    } else {
        r = -2;
    }
    return r;
}

int fio_close(int fd) {
    int r = 0;
//    DBGOUT("fio_close(%i)\r\n", fd);
    if (fio_is_open_int(fd) && (fd >= 3)) {
        if (fio_fds[fd].fdclose)
            r = fio_fds[fd].fdclose(fio_fds[fd].opaque);
        xSemaphoreTake(fio_sem, portMAX_DELAY);
        memset(fio_fds + fd, 0, sizeof(struct fddef_t));
        xSemaphoreGive(fio_sem);
    } else {
        r = -2;
    }
    return r;
}

void fio_set_opaque(int fd, void * opaque) {
    if (fio_is_open_int(fd))
        fio_fds[fd].opaque = opaque;
}

static int stdio_open(void * opaque, int flags, int mode) {
    uintptr_t fd = (uintptr_t) opaque;
    int ret = -1;
    fdread_t stdio_read;
    fdwrite_t stdio_write;
    void * opaque_stdin, * opaque_stdout;

    xSemaphoreTake(fio_sem, portMAX_DELAY);
    stdio_read = fio_fds[0].fdread;
    stdio_write = fio_fds[1].fdwrite;
    opaque_stdin = fio_fds[0].opaque;
    opaque_stdout = fio_fds[1].opaque;
    xSemaphoreGive(fio_sem);

    switch(fd) {
    case 0:
        ret = fio_open(stdio_read, NULL, NULL, NULL, opaque_stdin);
        break;
    case 1:
    case 2:
        ret = fio_open(NULL, stdio_write, NULL, NULL, opaque_stdout);
        break;
    }

    return ret;
}

void register_stdio_devices() {
    register_device("stdin", stdio_open, (void *)(uintptr_t) 0);
    register_device("stdout", stdio_open, (void *)(uintptr_t) 1);
    register_device("stderr", stdio_open, (void *)(uintptr_t) 2);
}

void register_custom_stdin(fdread_t custom_stdin_read, void * opaque) {
    xSemaphoreTake(fio_sem, portMAX_DELAY);
    if (custom_stdin_read) {
        fio_fds[0].fdread = custom_stdin_read;
        fio_fds[0].opaque = opaque;
    } else {
        fio_fds[0].fdread = stdin_read;
        fio_fds[0].opaque = NULL;
    }
    xSemaphoreGive(fio_sem);
}

void register_custom_stdout(fdwrite_t custom_stdout_write, void * opaque) {
    xSemaphoreTake(fio_sem, portMAX_DELAY);
    if (custom_stdout_write) {
        fio_fds[1].fdwrite = custom_stdout_write;
        fio_fds[1].opaque = opaque;
    } else {
        fio_fds[1].fdwrite = stdout_write;
        fio_fds[1].opaque = NULL;
    }
    xSemaphoreGive(fio_sem);
}

int dev_setblock(int fd, int enable) {
//    DBGOUT("fio_read(%i, %p, %i)\r\n", fd, buf, count);
    if (fio_is_open_int(fd)) {
        if (fio_fds[fd].setblock) {
            fio_fds[fd].setblock(fio_fds[fd].opaque, enable);
        } else {
            return -3;
        }
    } else {
        return -2;
    }
    return 0;
}

