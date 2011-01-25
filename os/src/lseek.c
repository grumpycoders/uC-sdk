#include <reent.h>

_off_t _lseek_r(struct _reent * reent, int fd, _off_t seek, int wheel) {
    return 0;
}
