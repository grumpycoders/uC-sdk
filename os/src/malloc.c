#include <reent.h>

void * malloc(size_t size) {
    return _malloc_r(_impure_ptr, size);
}
