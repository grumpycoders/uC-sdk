#include <stdio.h>

static FILE _stdin, _stdout, _stderr;
FILE * stdin = &_stdin, * stdout = &_stdout, * stderr = &_stderr;

void __sinit(struct _reent * reent) {
    stdin->fd = 0;
    stdout->fd = 1;
    stderr->fd = 2;
}
