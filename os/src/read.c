#include <reent.h>
#include <osdebug.h>
#include <errno.h>
#include "fio.h"

_ssize_t _read_r(struct _reent * reent, int fd, void * buf, size_t size) {
    _ssize_t r;
    
//    DBGOUT("_read_r(%p, %i, %p, %i)\r\n", reent, fd, buf, size);
    
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return -1;
    }
    
    r = fio_read(fd, buf, size);
    
    if (r < 0) {
        reent->_errno = EINVAL;
        return -1;
    }
    
    return r;
}
