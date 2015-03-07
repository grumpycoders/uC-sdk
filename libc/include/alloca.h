#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

static inline void * alloca(unsigned size) { return __builtin_alloca(size); }

END_DECL
