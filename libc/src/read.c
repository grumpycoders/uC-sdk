#include "reent.h"
#include "errno.h"
#include <fio.h>

ssize_t read(int fd, void * buf, size_t size) {
    ssize_t r;
    
    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }
    
    r = fio_read(fd, buf, size);
    
    if (r < 0) {
        set_errno(EINVAL);
        return -1;
    }
    
    return r;
}
