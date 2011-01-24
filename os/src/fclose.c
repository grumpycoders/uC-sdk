#include <stdio.h>
#include <reent.h>

int fclose(FILE * fp) {
    return _fclose_r(_impure_ptr, fp);
}
