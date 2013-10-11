TARGET_LIB = libos.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/libc/config.mk

TARGET_SRCS = \
src/init.c \
src/hooks.c \
\
src/filesystem.c \
src/fio.c \
src/hash-djb2.c \
src/osdebug.c \
src/romfs.c \
\
src/sbrk.c \

ifeq ($(CPU),arm)
TARGET_SRCS += src/semifs.c
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
