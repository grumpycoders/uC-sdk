#include <reent.h>
#include <sys/stat.h>
#include "osdebug.h"

int _fstat_r(struct _reent * ptr, int fildes, struct stat * buf) {
    DBGOUT("_fstat_r(%p, %d, %p)\r\n", ptr, fildes, buf);
    return 0;
}
