#include <unistd.h>
#include <reent.h>
#include "osdebug.h"

int _isatty_r(struct _reent * ptr, int fildes) {
    DBGOUT("_isatty_r(%p, %d)\r\n", ptr, fildes);
    return 0;
}
