#pragma once

#include <unistd.h>

uint32_t hash_djb2(const uint8_t * str, ssize_t max);
