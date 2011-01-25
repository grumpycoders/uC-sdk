#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include "BoardConsole.h"
#include <debug_frmwrk.h>

void BoardConsoleInit() {
    debug_frmwrk_init();
}

void BoardConsolePuts(const char * str) {
    _DBG_(str);
}

void BoardConsolePutc(int c) {
    _DBC(c);
}

void BoardConsolePrintf(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    BoardConsoleVPrintf(fmt, ap);
    va_end(ap);
}

void BoardConsoleVPrintf(const char * fmt, va_list ap) {
    char * str = NULL;
    vasprintf(&str, fmt, ap);
    _DBG(str);
    free(str);
}
