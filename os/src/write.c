#include <reent.h>
#include <osdebug.h>

_ssize_t _write_r(struct _reent * reent, int fd, const void * buf, size_t size) {
    DBGOUT("_write_r(%p, %d, %p, %u)\r\n", reent, fd, buf, size);
    return 0;
}
