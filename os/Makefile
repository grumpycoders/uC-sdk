TARGET_LIB = libos.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/arch/config.mk

TARGET_SRCS = src/init.c src/sbrk.c src/sprintf.c src/malloc.c src/free.c src/fclose.c src/read.c src/lseek.c src/write.c src/close.c src/osdebug.c \
src/printf.c src/fstat.c src/isatty.c src/fprintf.c

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
