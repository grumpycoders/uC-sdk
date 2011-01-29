#include <reent.h>
#include <osdebug.h>
#include <errno.h>
#include "fio.h"

int _close_r(struct _reent * reent, int fd) {
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return -1;
    }
    
    return fio_close(fd);
}
