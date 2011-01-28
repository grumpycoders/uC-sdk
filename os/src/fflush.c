#include <stdio.h>

int fflush(FILE * file) {
    return _fflush_r(_impure_ptr, file);
}
