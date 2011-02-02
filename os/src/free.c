#include <reent.h>
#include <malloc.h>
#include "osdebug.h"

void free(void * ptr) {
//    DBGOUT("free(%p)\r\n", ptr);
    _free_r(_impure_ptr, ptr);
}
