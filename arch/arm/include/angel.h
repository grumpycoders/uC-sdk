#pragma once

#include <decl.h>

BEGIN_DECL

int Semihost_SYS_OPEN(uint32_t * args);
int Semihost_SYS_CLOSE(uint32_t * args);
int Semihost_SYS_WRITE(uint32_t * args);
int Semihost_SYS_READ(uint32_t * args);
int Semihost_SYS_SEEK(uint32_t * args);
int Semihost_SYS_FLEN(uint32_t * args);

END_DECL
