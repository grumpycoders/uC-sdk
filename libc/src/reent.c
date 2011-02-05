#include "reent.h"

static struct _reent main_thread;
struct _reent * _impure_ptr = &main_thread;

__attribute__((constructor)) static void init_main_thread() {
    _REENT_INIT_PTR(_impure_ptr);
}
