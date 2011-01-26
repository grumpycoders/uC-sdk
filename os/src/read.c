#include <reent.h>
#include <osdebug.h>

_ssize_t _read_r(struct _reent * reent, int fd, void * ptr, size_t size) {
    DBGOUT("_read_r(%p, %d, %p, %u)\r\n", reent, fd, ptr, size);
    return 0;
}
