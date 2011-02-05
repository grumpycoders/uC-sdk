#include "reent.h"
#include "errno.h"
#include <fio.h>

ssize_t write(int fd, const void * buf, size_t size) {
    ssize_t r;

    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }
    
    r = fio_write(fd, buf, size);
    
    if (r < 0) {
        set_errno(EINVAL);
        return -1;
    }
    
    return r;
}
