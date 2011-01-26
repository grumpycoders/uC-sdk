#include <stdio.h>
#include <reent.h>
#include <osdebug.h>

int fclose(FILE * fp) {
    DBGOUT("fclose(%p)\r\n", fp);
    return _fclose_r(_impure_ptr, fp);
}
