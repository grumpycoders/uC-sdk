#include <reent.h>
#include <malloc.h>

void free(void * ptr) {
    _free_r(_impure_ptr, ptr);
}
