#include <reent.h>

void free(void * ptr) {
    _free_r(_impure_ptr, ptr);
}
