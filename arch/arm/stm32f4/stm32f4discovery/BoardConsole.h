#pragma once

#include <stdarg.h>

void BoardConsoleInit();
void BoardConsolePuts(const char * str);
void BoardConsolePutc(int);
void BoardConsolePrintf(const char * fmt, ...) __attribute__ ((format(printf, 1, 2)));
void BoardConsoleVPrintf(const char * fmt, va_list ap);
