#include "reent.h"
#include "errno.h"
#include <fio.h>

off_t lseek(int fd, off_t seek, int wheel) {
    off_t r;
    
    if ((wheel != SEEK_SET) && (wheel != SEEK_CUR) && (wheel != SEEK_END)) {
        set_errno(EINVAL);
        return -1;
    }
    
    if (!fio_is_open(fd)) {
        set_errno(EBADF);
        return -1;
    }
    
    r = fio_seek(fd, seek, wheel);
    
    if (r < 0)
        set_errno(EINVAL);
    
    return r;
}
