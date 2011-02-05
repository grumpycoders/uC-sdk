TARGET = demo.bin
LIBDEPS = FreeRTOS/libFreeRTOS.a arch/libarch.a os/libos.a libc/libc.a
LIBS = -Wl,--start-group $(LIBDEPS) -Wl,--end-group

TARGET_SRCS = test-romfs.o

export ROOTDIR = $(CURDIR)

include common.mk

all: tools libs $(TARGET)

clean: clean-generic
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C os clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C libc clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools clean
	$(Q)rm -f test-romfs.bin

.PHONY: libs FreeRTOS arch os libc tools deps

FreeRTOS/libFreeRTOS.a: FreeRTOS
arch/libarch.a: arch
os/libos.a: os
libc/libc.a: libc

libs: FreeRTOS arch os libc

FreeRTOS:
	$(E) "[MAKE]   Entering FreeRTOS"
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS

arch:
	$(E) "[MAKE]   Entering arch"
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch

os:
	$(E) "[MAKE]   Entering os"
	$(Q)$(MAKE) $(MAKE_OPTS) -C os

libc:
	$(E) "[MAKE]   Entering libc"
	$(Q)$(MAKE) $(MAKE_OPTS) -C libc

tools:
	$(E) "[MAKE]   Entering tools"
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools

test-romfs.o: tools/mkromfs
	$(E) "[ROMFS]  Building test romfs"
	$(Q) tools/mkromfs -d test-romfs test-romfs.bin
	$(Q) $(TARGET_OBJCOPY_BIN) --prefix-sections '.romfs' test-romfs.bin test-romfs.o
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools

deps: ldeps
	$(E) "[DEPS]   Creating dependency tree for FreeRTOS"
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS ldeps
	$(E) "[DEPS]   Creating dependency tree for arch"
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch ldeps
	$(E) "[DEPS]   Creating dependency tree for os"
	$(Q)$(MAKE) $(MAKE_OPTS) -C os ldeps
	$(E) "[DEPS]   Creating dependency tree for libc"
	$(Q)$(MAKE) $(MAKE_OPTS) -C libc ldeps

include FreeRTOS/config.mk
include arch/config.mk
include os/config.mk
include libc/config.mk
include target-rules.mk
