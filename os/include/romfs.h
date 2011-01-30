#ifndef __ROMFS_H__
#define __ROMFS_H__

#include <stdint.h>

void register_romfs(const char * mountpoint, const uint8_t * romfs);

#endif

