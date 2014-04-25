#include <BoardConsole.h>
#include <string.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <unistd.h>
#include "fio.h"
#include "devfs.h"
#include "filesystem.h"
#include "osdebug.h"
#include "hash-djb2.h"

struct dev_t {
    uint32_t hash;
    dev_open_t cb;
    void * opaque;
};

static struct dev_t devs[MAX_FS];

static int devfs_open(void * opaque, const char * path, int flags, int mode) {
    uint32_t hash;
    int i;

    hash = hash_djb2((const uint8_t *) path, -1);

    for (i = 0; i < MAX_DEVS; i++) {
        if (devs[i].hash == hash)
            return devs[i].cb(devs[i].opaque, flags, mode);
    }

    return -2;
}

void register_devfs() {
    DBGOUT("Registering devfs.\r\n");
    register_fs("dev", devfs_open, NULL);
}

int register_device(const char * name, dev_open_t devopen, void * opaque) {
    int i;
    DBGOUT("register_device(\"%s\", %p, %p)\r\n", name, devopen, opaque);

    for (i = 0; i < MAX_DEVS; i++) {
        if (!devs[i].cb) {
            devs[i].hash = hash_djb2((const uint8_t *) name, -1);
            devs[i].cb = devopen;
            devs[i].opaque = opaque;
            return 0;
        }
    }

    return -1;
}
