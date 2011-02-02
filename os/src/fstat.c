#include <reent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "fio.h"
#include "osdebug.h"

int _fstat_r(struct _reent * reent, int fd, struct stat * buf) {
    off_t c;
//    DBGOUT("_fstat_r(%p, %i, %p)\r\n", reent, fd, buf);
    memset(buf, 0, sizeof(struct stat));
    
    if (!fio_is_open(fd)) {
        reent->_errno = EBADF;
        return -1;
    }
    
    buf->st_mode = S_IFCHR;
    buf->st_blksize = 1024;
    c = fio_seek(fd, 0, SEEK_CUR);
    if (c >= 0) {
        buf->st_size = fio_seek(fd, 0, SEEK_END);
        fio_seek(fd, c, SEEK_SET);
    }

    return 0;
}
