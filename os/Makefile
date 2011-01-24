TARGET_LIB = libos.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk

TARGET_SRCS = src/init.c

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
