export ROOTDIR = $(CURDIR)

include common.mk

all: tools libs $(TARGET)

clean: clean-generic
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C hardware clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C os clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C libc clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C libm clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C acorn clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C lwip clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C chips clean

.PHONY: libs FreeRTOS arch chips hardware os libc libm acorn lwip tools deps

FreeRTOS/libFreeRTOS.a: FreeRTOS
arch/libarch.a: arch
hardware/libhardware.a: hardware
os/libos.a: os
libc/libc.a: libc
libm/libm.a: libm
acorn/libacorn.a: acorn
lwip/liblwip.a: lwip
chips/libchips.a: chips

libs: FreeRTOS arch hardware os libc libm acorn lwip

FreeRTOS:
	$(E) "[MAKE]   Entering FreeRTOS"
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS

arch:
	$(E) "[MAKE]   Entering arch"
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch

hardware:
	$(E) "[MAKE]   Entering hardware"
	$(Q)$(MAKE) $(MAKE_OPTS) -C hardware

os:
	$(E) "[MAKE]   Entering os"
	$(Q)$(MAKE) $(MAKE_OPTS) -C os

libc:
	$(E) "[MAKE]   Entering libc"
	$(Q)$(MAKE) $(MAKE_OPTS) -C libc

libm:
	$(E) "[MAKE]   Entering libm"
	$(Q)$(MAKE) $(MAKE_OPTS) -C libm

acorn:
	$(E) "[MAKE]   Entering acorn"
	$(Q)$(MAKE) $(MAKE_OPTS) -C acorn

lwip:
	$(E) "[MAKE]   Entering lwip"
	$(Q)$(MAKE) $(MAKE_OPTS) -C lwip

chips:
	$(E) "[MAKE]   Entering chips"
	$(Q)$(MAKE) $(MAKE_OPTS) -C chips

tools:
	$(E) "[MAKE]   Entering tools"
	$(Q)$(MAKE) $(MAKE_OPTS) -C tools

deps: ldeps
	$(E) "[DEPS]   Creating dependency tree for FreeRTOS"
	$(Q)$(MAKE) $(MAKE_OPTS) -C FreeRTOS ldeps
	$(E) "[DEPS]   Creating dependency tree for arch"
	$(Q)$(MAKE) $(MAKE_OPTS) -C arch ldeps
	$(E) "[DEPS]   Creating dependency tree for hardware"
	$(Q)$(MAKE) $(MAKE_OPTS) -C hardware ldeps
	$(E) "[DEPS]   Creating dependency tree for os"
	$(Q)$(MAKE) $(MAKE_OPTS) -C os ldeps
	$(E) "[DEPS]   Creating dependency tree for libc"
	$(Q)$(MAKE) $(MAKE_OPTS) -C libc ldeps
	$(E) "[DEPS]   Creating dependency tree for libm"
	$(Q)$(MAKE) $(MAKE_OPTS) -C libm ldeps
	$(E) "[DEPS]   Creating dependency tree for acorn"
	$(Q)$(MAKE) $(MAKE_OPTS) -C acorn ldeps
	$(E) "[DEPS]   Creating dependency tree for lwip"
	$(Q)$(MAKE) $(MAKE_OPTS) -C lwip ldeps
	$(E) "[DEPS]   Creating dependency tree for chips"
	$(Q)$(MAKE) $(MAKE_OPTS) -C chips ldeps

include arch/config.mk
include FreeRTOS/config.mk
include hardware/config.mk
include os/config.mk
include libc/config.mk
include libm/config.mk
include lwip/config.mk
include acorn/config.mk
include chips/config.mk
include target-rules.mk
