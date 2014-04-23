#pragma once

#include <stdint.h>

void register_romfs(const char * mountpoint, const uint8_t * romfs);
const uint8_t * romfs_get_file_by_hash(const uint8_t * romfs, uint32_t h, uint32_t * len);
