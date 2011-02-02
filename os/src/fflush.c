#include <stdio.h>
#include "osdebug.h"

int fflush(FILE * file) {
//    DBGOUT("fflush(%p)\r\n", file);
    return _fflush_r(_impure_ptr, file);
}
