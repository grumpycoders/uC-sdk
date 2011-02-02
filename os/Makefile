TARGET_LIB = libos.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/arch/config.mk

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
src/close.c \
src/fstat.c \
src/isatty.c \
src/lseek.c \
src/open.c \
src/read.c \
src/sbrk.c \
src/write.c \
\
src/fclose.c \
src/fflush.c \
src/fopen.c \
src/fprintf.c \
src/fread.c \
src/free.c \
src/fwrite.c \
src/malloc.c \
src/printf.c \
src/sprintf.c \

ifeq ($(CPU),arm)
TARGET_SRCS += src/semifs.c
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
