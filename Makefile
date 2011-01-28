TARGET = demo.bin
LIBDEPS = FreeRTOS/libFreeRTOS.a arch/libarch.a os/libos.a
LIBS = -Wl,--start-group -lc $(LIBDEPS) -Wl,--end-group

TARGET_SRCS = extern.c

export ROOTDIR = $(CURDIR)

include common.mk

all: libs $(TARGET)

clean: clean-generic
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C os clean

.PHONY: libs FreeRTOS arch os

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

include FreeRTOS/config.mk
include arch/config.mk
include os/config.mk
include target-rules.mk
