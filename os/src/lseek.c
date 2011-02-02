#include <reent.h>
#include <osdebug.h>
#include <errno.h>
#include "fio.h"
#include "osdebug.h"

_off_t _lseek_r(struct _reent * reent, int fd, _off_t seek, int wheel) {
    off_t r;
//    DBGOUT("_lseek_r(%p, %i, %i, %i)\r\n", reent, fd, seek, wheel);
    
    if ((wheel != SEEK_SET) && (wheel != SEEK_CUR) && (wheel != SEEK_END)) {
        reent->_errno = EINVAL;
        return -1;
    }
    
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return -1;
    }
    
    r = fio_seek(fd, seek, wheel);
    
    if (r < 0)
        reent->_errno = EINVAL;
    
    return r;
}
