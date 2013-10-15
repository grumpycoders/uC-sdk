TARGET_LIB = libc.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/arch/config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libm/config.mk

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
src/errno.c \
\
src/qsort.c \
\
src/reent.c \
\
src/initfini.c \
\
src/malloc.c \
\
src/xprintf.c \
src/xscanf.c \
src/yscanf.c \

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
