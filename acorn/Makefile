TARGET_LIB = libacorn.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libc/config.mk
include $(ROOTDIR)/libm/config.mk

TARGET_SRCS = \
src/malloc_wrapper.c \

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
