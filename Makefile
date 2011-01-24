TARGET = demo.bin
LIBS = -Wl,--start-group -lc FreeRTOS/libFreeRTOS.a arch/libarch.a os/libos.a -Wl,--end-group

export ROOTDIR = $(CURDIR)

include common.mk

all: libs $(TARGET)

clean: clean-generic
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C os clean

.PHONY: libs FreeRTOS arch os

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
