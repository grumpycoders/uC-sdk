#pragma once

#include <BoardConsole.h>

void osDbgPrintf(const char * fmt, ...);

#ifdef FULLDEBUG
#define DBGOUT osDbgPrintf
#else
#define DBGOUT(...)
#endif
