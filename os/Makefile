TARGET_LIB = libos.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk

TARGET_SRCS = src/init.c src/sbrk.c

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
