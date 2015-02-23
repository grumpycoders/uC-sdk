#pragma once

#include <decl.h>
#include <stdint.h>

BEGIN_DECL

void register_romfs(const char * mountpoint, const uint8_t * romfs);
const uint8_t * romfs_get_file_by_hash(const uint8_t * romfs, uint32_t h, uint32_t * len);
const uint8_t * romfs_get_file(const uint8_t * romfs, const char * path, uint32_t * len);

END_DECL
