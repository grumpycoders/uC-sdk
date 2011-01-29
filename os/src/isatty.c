#include <unistd.h>
#include <reent.h>
#include <errno.h>
#include "fio.h"

int _isatty_r(struct _reent * reent, int fd) {
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return 0;
    }
    reent->_errno = EINVAL;
    return 0;
}
