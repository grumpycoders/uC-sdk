TARGET_LIB = libc.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/arch/config.mk
include $(ROOTDIR)/os/config.mk

ifneq ($(USE_FLOATINGPOINT),true)
TARGET_CPPFLAGS = -DNOFLOATINGPOINT
endif

TARGET_SRCS = \
src/close.c \
src/lseek.c \
src/open.c \
src/read.c \
src/write.c \
\
src/stdio.c \
\
src/reent.c \
\
src/exit.c \
\
src/malloc.c \
\
src/xprintf.c \

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
