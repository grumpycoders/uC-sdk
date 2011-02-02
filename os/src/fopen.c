#include <stdio.h>
#include <reent.h>
#include "osdebug.h"

FILE * fopen(const char * path, const char * mode) {
//    DBGOUT("fopen(\"%s\", \"%s\")\r\n", path, mode);
    return _fopen_r(_impure_ptr, path, mode);
}
