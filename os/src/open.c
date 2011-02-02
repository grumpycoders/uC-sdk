#include <stdio.h>
#include <reent.h>
#include <errno.h>
#include "fio.h"
#include "filesystem.h"
#include "osdebug.h"

int _open_r(struct _reent * reent, const char * path, int flags, int mode) {
//    DBGOUT("_open_r(%p, \"%s\", flags, mode)\r\n", reent, path, flags, mode);
    int r = fs_open(path, flags, mode);
    
    if (r >= 0)
        return r;
    
    reent->_errno = EACCES;
    return -1;
}
