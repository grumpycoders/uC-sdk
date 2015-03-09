TARGET_LIB = libchips.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/libc/config.mk
include $(ROOTDIR)/hardware/config.mk

TARGET_SRCS = \
src/sdcard.c \
src/l3gd20.c \
src/lis3dsh.c

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
