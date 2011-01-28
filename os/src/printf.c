#include <reent.h>
#include <stdio.h>
#include <stdarg.h>
#include <osdebug.h>

int printf(const char * fmt, ...) {
    int r;
    va_list ap;
    DBGOUT("printf(%p, ...)\r\n", fmt);
    va_start(ap, fmt);
    r = _vprintf_r(_impure_ptr, fmt, ap);
    va_end(ap);
    return r;
}
