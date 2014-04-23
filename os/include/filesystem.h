#pragma once

#include <decl.h>
#include <stdint.h>
#include <hash-djb2.h>

#define MAX_FS 16

BEGIN_DECL

typedef int (*fs_open_t)(void * opaque, const char * fname, int flags, int mode);

int register_fs(const char * mountpoint, fs_open_t callback, void * opaque);
int fs_open(const char * path, int flags, int mode);

END_DECL
