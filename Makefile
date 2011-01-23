export ROOTDIR = $(CURDIR)

include $(ROOTDIR)/common.mk

all: libs

clean:
	$(MAKE) -C FreeRTOS clean
	$(MAKE) -C arch clean


.PHONY: libs FreeRTOS arch

libs: FreeRTOS arch

FreeRTOS:
	$(MAKE) -C FreeRTOS

arch:
	$(MAKE) -C arch

