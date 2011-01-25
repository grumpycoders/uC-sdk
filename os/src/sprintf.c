#include <reent.h>
#include <stdio.h>
#include <stdarg.h>

int sprintf(char * str, const char * fmt, ...) {
    int r;
    va_list ap;
    va_start(ap, fmt);
    r = _vsprintf_r(_impure_ptr, str, fmt, ap);
    va_end(ap);
    return r;
}
