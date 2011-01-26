#include <reent.h>
#include <malloc.h>
#include <osdebug.h>

void * malloc(size_t size) {
    DBGOUT("malloc(%u)\r\n", size);
    return _malloc_r(_impure_ptr, size);
}
