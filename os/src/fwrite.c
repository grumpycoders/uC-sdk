#include <stdio.h>

size_t fwrite(const void * ptr, size_t size, size_t nmemb, FILE * file) {
    return _fwrite_r(_impure_ptr, ptr, size, nmemb, file);
}
