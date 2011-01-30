#include <stdio.h>

size_t fread(void * ptr, size_t size, size_t nmemb, FILE * file) {
    return _fread_r(_impure_ptr, ptr, size, nmemb, file);
}
