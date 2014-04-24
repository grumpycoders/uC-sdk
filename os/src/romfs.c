#include <string.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <unistd.h>
#ifdef USE_NEWLIB
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "fio.h"
#include "filesystem.h"
#include "romfs.h"
#include "osdebug.h"
#include "hash-djb2.h"

struct romfs_fds_t {
    const uint8_t * file;
    uint32_t cursor;
};

static struct romfs_fds_t romfs_fds[MAX_FDS];

static uint32_t get_unaligned(const uint8_t * d) {
    return ((uint32_t) d[0]) | ((uint32_t) (d[1] << 8)) | ((uint32_t) (d[2] << 16)) | ((uint32_t) (d[3] << 24));
}

static ssize_t romfs_read(void * opaque, void * buf, size_t count) {
    struct romfs_fds_t * f = (struct romfs_fds_t *) opaque;
    const uint8_t * size_p = f->file - 4;
    uint32_t size = get_unaligned(size_p);
    
    if ((f->cursor + count) > size)
        count = size - f->cursor;

    memcpy(buf, f->file + f->cursor, count);
    f->cursor += count;

    return count;
}

static off_t romfs_seek(void * opaque, off_t offset, int whence) {
    struct romfs_fds_t * f = (struct romfs_fds_t *) opaque;
    const uint8_t * size_p = f->file - 4;
    uint32_t size = get_unaligned(size_p);
    uint32_t origin;
    
    switch (whence) {
    case SEEK_SET:
        origin = 0;
        break;
    case SEEK_CUR:
        origin = f->cursor;
        break;
    case SEEK_END:
        origin = size;
        break;
    default:
        return -1;
    }

    offset = origin + offset;

    if (offset < 0)
        return -1;
    if (offset > size)
        offset = size;

    f->cursor = offset;

    return offset;
}

const uint8_t * romfs_get_file_by_hash(const uint8_t * romfs, uint32_t h, uint32_t * len) {
    const uint8_t * meta;
    uint32_t hash;
    uint32_t flen;

    meta = romfs;
    do {
        hash = get_unaligned(meta);
        flen = get_unaligned(meta + 4);
        if (hash == h) {
            if (len)
                *len = get_unaligned(meta + 4);
            return meta + 8;
        }
        meta += flen + 8;
    } while ((hash != 0) || (flen != 0));

    return NULL;
}

static int romfs_open(void * opaque, const char * path, int flags, int mode) {
    uint32_t h = hash_djb2((const uint8_t *) path, -1);
    const uint8_t * romfs = (const uint8_t *) opaque;
    const uint8_t * file;
    int r = -1;

    file = romfs_get_file_by_hash(romfs, h, NULL);

    if (file) {
        r = fio_open(romfs_read, NULL, romfs_seek, NULL, NULL);
        if (r > 0) {
            romfs_fds[r].file = file;
            romfs_fds[r].cursor = 0;
            fio_set_opaque(r, romfs_fds + r);
        }
    }
    return r;
}

void register_romfs(const char * mountpoint, const uint8_t * romfs) {
//    DBGOUT("Registering romfs `%s' @ %p\r\n", mountpoint, romfs);
    register_fs(mountpoint, romfs_open, (void *) romfs);
}
