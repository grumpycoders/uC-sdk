#pragma once

#include <errno.h>

struct _reent {
    enum errno_t _errno;
};

static __inline__ void _REENT_INIT_PTR(struct _reent * reent) { reent->_errno = ENOERROR; }
extern struct _reent * _impure_ptr;
static __inline__ void set_errno(enum errno_t _errno) { _impure_ptr->_errno = errno = _errno; }
