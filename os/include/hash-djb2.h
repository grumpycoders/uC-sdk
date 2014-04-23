#pragma once

#include <decl.h>
#include <unistd.h>

BEGIN_DECL

uint32_t hash_djb2(const uint8_t * str, ssize_t max);

END_DECL
