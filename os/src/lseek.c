#include <reent.h>
#include <osdebug.h>

_off_t _lseek_r(struct _reent * reent, int fd, _off_t seek, int wheel) {
    DBGOUT("_lseek_r(%p, %d, %d, %d)\r\n", reent, fd, (int) seek, wheel);
    return 0;
}
