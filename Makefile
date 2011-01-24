TARGET = demo.bin
LIBS = FreeRTOS/libFreeRTOS.a arch/libarch.a -lc os/libos.a

export ROOTDIR = $(CURDIR)

include common.mk

all: libs $(TARGET)

clean: clean-generic
	$(MAKE) -C FreeRTOS clean
	$(MAKE) -C arch clean
	$(MAKE) -C os clean

.PHONY: libs FreeRTOS arch os

libs: FreeRTOS arch os

FreeRTOS:
	$(MAKE) -C FreeRTOS

arch:
	$(MAKE) -C arch

os:
	$(MAKE) -C os

include FreeRTOS/config.mk
include arch/config.mk
include os/config.mk
include target-rules.mk
