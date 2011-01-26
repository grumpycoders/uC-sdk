#include <reent.h>
#include <osdebug.h>

int _close_r(struct _reent * reent, int fd) {
    DBGOUT("_close_r(%p, %d)\r\n", reent, fd);
    return 0;
}
