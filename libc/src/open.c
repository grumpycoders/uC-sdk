#include "stdio.h"
#include "reent.h"
#include "errno.h"
#include <fio.h>
#include <filesystem.h>

int open(const char * path, int flags) {
    int r;
    
    r = fs_open(path, flags, 0755);
    
    if (r >= 0)
        return r;
    
    _impure_ptr->_errno = EACCES;
    return -1;
}
