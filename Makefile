TARGET = demo.bin
LIBS = -LFreeRTOS -Larch -lFreeRTOS -larch

export ROOTDIR = $(CURDIR)

include common.mk

all: libs $(TARGET)

clean: clean-generic
	$(MAKE) -C FreeRTOS clean
	$(MAKE) -C arch clean

.PHONY: libs FreeRTOS arch

libs: FreeRTOS arch

FreeRTOS:
	$(MAKE) -C FreeRTOS

arch:
	$(MAKE) -C arch

include FreeRTOS/config.mk
include arch/config.mk
include target-rules.mk
