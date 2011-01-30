TARGET = demo.bin
LIBDEPS = FreeRTOS/libFreeRTOS.a arch/libarch.a os/libos.a
LIBS = -Wl,--start-group -lc $(LIBDEPS) -Wl,--end-group

TARGET_SRCS = test-romfs.o

export ROOTDIR = $(CURDIR)

include common.mk

all: tools libs $(TARGET)

clean: clean-generic
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C os clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools clean

.PHONY: libs FreeRTOS arch os tools

FreeRTOS/libFreeRTOS.a: FreeRTOS
arch/libarch.a: arch
os/libos.a: os

libs: FreeRTOS arch os

FreeRTOS:
	$(E) "[MAKE]   Entering FreeRTOS"
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS

arch:
	$(E) "[MAKE]   Entering arch"
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch

os:
	$(E) "[MAKE]   Entering os"
	$(Q)$(MAKE) $(MAKE_OPTS) -C os

tools:
	$(E) "[MAKE]   Entering tools"
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools

test-romfs.o: tools
	$(E) "[ROMFS]  Building test romfs"
	$(Q) tools/mkromfs -d test test-romfs.bin
	$(Q) $(TARGET_OBJCOPY_BIN) --prefix-sections '.romfs' test-romfs.bin test-romfs.o
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools

include FreeRTOS/config.mk
include arch/config.mk
include os/config.mk
include target-rules.mk
