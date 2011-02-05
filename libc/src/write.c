#include "reent.h"
#include "errno.h"
#include <fio.h>

ssize_t write(int fd, const void * buf, size_t size) {
    ssize_t r;

    if (!fio_is_open(fd)) {
        _impure_ptr->_errno = EBADF;
        return -1;
    }
    
    r = fio_write(fd, buf, size);
    
    if (r < 0) {
        _impure_ptr->_errno = EINVAL;
        return -1;
    }
    
    return r;
}
