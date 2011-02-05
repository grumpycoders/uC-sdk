/* Copyright (c) 2010 James Snyder, eLua Project.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the right
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <string.h>
#include <errno.h>
#include <stdint.h>
#ifdef USE_NEWLIB
#include <fcntl.h>
#endif

#include <angel.h>

#include "filesystem.h"
#include "fio.h"
#include "semifs.h"
#include "osdebug.h"

struct semifs_fds_t {
    uint32_t file;
    uint32_t cursor;
};

static struct semifs_fds_t semifs_fds[MAX_FDS];

// These structures provided by Simon Ford of mbed
struct semifs_FTime {
    uint8_t  hr;                        // Hours    [0..23]
    uint8_t  min;                       // Minutes  [0..59]
    uint8_t  sec;                       // Seconds  [0..59]
    uint8_t  day;                       // Day      [1..31]
    uint8_t  mon;                       // Month    [1..12]
    uint16_t year;                      // Year     [1980..2107]
};

// File Search info record
struct semifs_XF_Info {
    char name[32];                      // File - 32-bytes
    uint32_t size;                      // File size in bytes - 4-bytes
    uint16_t fileID;                    // System File Identification  - 2-bytes
    struct semifs_FTime create_time;    // Date & time file was created
    struct semifs_FTime write_time;     // Date & time of last write
};

struct semifs_SearchInfo {
    const char *pattern;
    struct semifs_XF_Info file_info;
};

/*****************************************************************************/

static int semifs_close(void * opaque) {
    struct semifs_fds_t * f = (struct semifs_fds_t *) opaque;
//    DBGOUT("semifs_close(%p)\r\n", opaque);
    
    return Semihost_SYS_CLOSE(&f->file);
}

static ssize_t semifs_write(void * opaque, const void * ptr, size_t len) {
    struct semifs_fds_t * f = (struct semifs_fds_t *) opaque;
//    DBGOUT("semifs_write(%p, %p, %i)\r\n", opaque, ptr, len);

    uint32_t args[3];
    args[0] = (uint32_t) f->file;
    args[1] = (uint32_t) ptr;
    args[2] = (uint32_t) len;

    // Perform write
    int res = Semihost_SYS_WRITE(args);
//    DBGOUT("semifs_read: Semihost_SYS_WRITE returned %i\r\n", res);
    if (res == -1 || res == len)
        return -1;

    // Update position
    f->cursor += len - res;
    return len - res;
}

static ssize_t semifs_read(void * opaque, void * ptr, size_t len) {
    struct semifs_fds_t * f = (struct semifs_fds_t *) opaque;
//    DBGOUT("semifs_read(%p, %p, %i)\r\n", opaque, ptr, len);

    uint32_t args[3];
    args[0] = (uint32_t) f->file;
    args[1] = (uint32_t) ptr;
    args[2] = (uint32_t) len;

    // Perform read
    int res = Semihost_SYS_READ(args);
    DBGOUT("semifs_read: Semihost_SYS_READ returned %i\r\n", res);
    if (res < 0)
        return -1;

    // Update position
    f->cursor += len - res;
    return len - res;
}

static off_t semifs_seek(void * opaque, off_t off, int whence) {
    uint32_t args[2], len;
    struct semifs_fds_t * f = (struct semifs_fds_t *) opaque;
//    DBGOUT("semifs_seek(%p, %i, %i)\r\n", opaque, off, whence);

    switch (whence) {
    case SEEK_CUR:
        // seek from current position
        off += f->cursor;
        whence = SEEK_SET;
        break;

    case SEEK_END:
        // seek from end of file
        args[0] = (uint32_t) f->file;
        len = Semihost_SYS_FLEN(args);
//        DBGOUT("semifs_seek: Semihost_SYS_FLEN returned %i\r\n", len);
        off += len;
        break;
    }
    // Do absolute seek
    args[0] = (uint32_t) f->file;
    args[1] = (uint32_t) off;
    int res = Semihost_SYS_SEEK(args);
//    DBGOUT("semifs_seek: Semihost_SYS_SEEK returned %i\r\n", res);

    if (res == 0)
        f->cursor = off;

    /* This is expected to return the position in the file.  */
    return res == 0 ? off : -1;
}

static int semifs_open(void * opaque, const char *path, int flags, int mode) {
    int aflags = 0;
    unsigned int fh;
    uint32_t args[3];
//    DBGOUT("semifs_open(%p, \"%s\", %i, %i)\r\n");

    if (flags & O_RDWR)
        aflags |= 2;

    if (flags & O_CREAT)
        aflags |= 4;

    if (flags & O_TRUNC)
        aflags |= 4;

    if (flags & O_APPEND) {
        aflags &= ~4; // Can't ask for w AND a; means just 'a'.
        aflags |= 8;
    }

    // Find file and open it (via semihosting call)
    args[0] = (uint32_t) path;
    args[1] = (uint32_t) aflags;
    args[2] = (uint32_t) strlen(path);
    fh = Semihost_SYS_OPEN(args);
//    DBGOUT("Semihost_SYS_OPEN returns: %i\r\n", fh);

    if (fh >= 0) {
        int fd = fio_open(semifs_read, semifs_write, semifs_seek, semifs_close, (void *) fh);
//        DBGOUT("semifs_open: fio_open returned %i\r\n", fd);
        if (fd < 0)
            Semihost_SYS_CLOSE((uint32_t *) &fd);
        semifs_fds[fd].file = fh;
        semifs_fds[fd].cursor = 0;
        fio_set_opaque(fd, semifs_fds + fd);
        return fd;
    }
    return -1;
}

void register_semifs() {
    DBGOUT("Registering semihost fs\r\n");
    register_fs("host", semifs_open, NULL);
}
