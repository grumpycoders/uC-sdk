#include <stdio.h>
#include "osdebug.h"

size_t fread(void * ptr, size_t size, size_t nmemb, FILE * file) {
//    DBGOUT("fread(%p, %i, %i, %p)\r\n", ptr, size, nmemb, file);
    return _fread_r(_impure_ptr, ptr, size, nmemb, file);
}
