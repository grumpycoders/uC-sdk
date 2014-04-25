#pragma once

#include <decl.h>
#include <stdio.h>
#include <fio.h>

BEGIN_DECL

#define MAX_DEVS 32

typedef void (*dev_setblock_t)(void * opaque, int enable);
typedef int (*dev_open_t)(void * opaque, int flags, int mode);

int dev_open(fdread_t, fdwrite_t, fdseek_t, fdclose_t, dev_setblock_t, void * opaque);
int dev_setblock(int fd, int enable);

void register_devfs();
int register_device(const char * name, dev_open_t devopen, void * opaque);

END_DECL
