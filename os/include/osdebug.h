#ifndef __OSDEBUG_H__
#define __OSDEBUG_H__

#include <BoardConsole.h>

void osDbgPrintf(const char * fmt, ...);

#ifdef FULLDEBUG
#define DBGOUT osDbgPrintf
#else
#define DBGOUT(...)
#endif

#endif
