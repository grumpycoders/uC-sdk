export NODEMO = true
export BOARD = stm32f4discovery
ROOTDIR = $(CURDIR)/uC-sdk

TARGET = test.bin
TARGET_OBJS = romfs.o

LIBDEPS = uC-sdk/FreeRTOS/libFreeRTOS.a uC-sdk/arch/libarch.a uC-sdk/os/libos.a uC-sdk/libc/libc.a uC-sdk/libm/libm.a uC-sdk/acorn/libacorn.a uC-sdk/hardware/libhardware.a
LIBS = -Wl,--start-group $(LIBDEPS) -Wl,--end-group
TARGET_INCLUDES = include

include $(ROOTDIR)/common.mk

all: uC-sdk $(TARGET)

clean: clean-generic
	$(Q)rm -f romfs.bin
	$(Q)$(MAKE) $(MAKE_OPTS) -C uC-sdk clean

.PHONY: uC-sdk

uC-sdk/FreeRTOS/libFreeRTOS.a: uC-sdk
uC-sdk/arch/libarch.a: uC-sdk
uC-sdk/os/libos.a: uC-sdk
uC-sdk/libc/libc.a: uC-sdk
uC-sdk/libm/libm.a: uC-sdk
uC-sdk/acorn/libacorn.a: uC-sdk
uC-sdk/hardware/libhardware.a: uC-sdk

uC-sdk:
	$(E) "[MAKE]   Entering uC-sdk"
	$(Q)$(MAKE) $(MAKE_OPTS) -C uC-sdk

romfs.o:
	$(E) "[ROMFS]  Building romfs"
	$(Q)uC-sdk/tools/romfs/mkromfs -d romfs romfs.bin
	$(Q)$(TARGET_OBJCOPY_BIN) --prefix-sections '.romfs' romfs.bin romfs.o

deps: ldeps
	$(E) "[DEPS]   Creating dependency tree for uC-sdk"
	$(Q)$(MAKE) $(MAKE_OPTS) -C uC-sdk ldeps

include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/arch/config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libc/config.mk
include $(ROOTDIR)/libm/config.mk
include $(ROOTDIR)/acorn/config.mk
include $(ROOTDIR)/hardware/config.mk
include $(ROOTDIR)/target-rules.mk

