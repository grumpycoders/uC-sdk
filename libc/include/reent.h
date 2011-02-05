#ifndef __RENT_H__
#define __RENT_H__

#include <errno.h>

struct _reent {
    enum errno_t _errno;
};

static inline void _REENT_INIT_PTR(struct _reent * reent) {
    reent->_errno = ENOERROR;
}

extern struct _reent * _impure_ptr;

#endif
