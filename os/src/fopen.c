#include <stdio.h>
#include <reent.h>

FILE * fopen(const char * path, const char * mode) {
    return _fopen_r(_impure_ptr, path, mode);
}
