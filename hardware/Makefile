TARGET_LIB = libhardware.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/libc/config.mk

TARGET_SRCS = \
src/sdcard.c \

ifeq ($(CPU_FAMILY),CM3)
TARGET_SRCS += src/gpio-cm3.c
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
