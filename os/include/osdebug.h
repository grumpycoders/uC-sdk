#pragma once

#include <decl.h>
#include <BoardConsole.h>

BEGIN_DECL

void osDbgPrintf(const char * fmt, ...);

END_DECL

#ifdef FULLDEBUG
#define DBGOUT osDbgPrintf
#else
#define DBGOUT(...)
#endif
