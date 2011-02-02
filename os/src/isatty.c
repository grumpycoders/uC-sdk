#include <unistd.h>
#include <reent.h>
#include <errno.h>
#include "fio.h"
#include "osdebug.h"

int _isatty_r(struct _reent * reent, int fd) {
//    DBGOUT("_isatty_r(%p, %i)\r\n", reent, fd);
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return 0;
    }
    reent->_errno = EINVAL;
    return 0;
}
