#include <string.h>
#include <errno.h>
#include <stdint.h>

#include <angel.h>

#include "fio.h"
#include "semiio.h"
#include "osdebug.h"

static ssize_t semi_stdin_read(void * opaque, void * buf, size_t count) {
    return 0;
}

static ssize_t semi_stdout_write(void * opaque, const void * buf, size_t count) {
    const uint8_t * buffer = buf;
    size_t ret = count;
    while (count--) {
        if (Semihost_SYS_WRITEC((uint32_t *) buffer++) != 0) {
            return -1;
        }
    }

    return ret;
}

void register_semiio() {
    register_custom_stdin(semi_stdin_read, NULL);
    register_custom_stdout(semi_stdout_write, NULL);
}
