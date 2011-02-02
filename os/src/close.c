#include <reent.h>
#include <errno.h>
#include "fio.h"
#include "osdebug.h"

int _close_r(struct _reent * reent, int fd) {
//    DBGOUT("_close_r(%p, %i)\r\n", reent, fd);
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return -1;
    }
    
    return fio_close(fd);
}
