#include <stdio.h>

FILE * stdin, * stdout, * stderr;

void __sinit(struct _reent * reent) {
    stdin->fd = 0;
    stdout->fd = 1;
    stderr->fd = 2;
}
