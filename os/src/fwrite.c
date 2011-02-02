#include <stdio.h>
#include "osdebug.h"

size_t fwrite(const void * ptr, size_t size, size_t nmemb, FILE * file) {
//    DBGOUT("fwrite(%p, %i, %i, %p)\r\n", ptr, size, nmemb, file);
    return _fwrite_r(_impure_ptr, ptr, size, nmemb, file);
}
