#include <reent.h>
#include <stdio.h>
#include <stdarg.h>
#include "osdebug.h"

int fprintf(FILE * file, const char * fmt, ...) {
    int r;
    va_list ap;
//    DBGOUT("fprintf(%p, %p, ...)\r\n", file, fmt);
    va_start(ap, fmt);
    r = _vfprintf_r(_impure_ptr, file, fmt, ap);
    va_end(ap);
    return r;
}
