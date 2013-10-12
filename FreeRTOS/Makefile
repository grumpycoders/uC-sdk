TARGET_LIB = libFreeRTOS.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libc/config.mk

TARGET_SRCS = Source/croutine.c Source/list.c Source/queue.c Source/tasks.c

TARGET_SRCS += Source/portable/MemMang/heap_3.c

ifeq ($(CPU),arm)
ifeq ($(CPU_FAMILY),CM3)
ifeq ($(USE_MPU),true)
TARGET_SRCS += Source/portable/GCC/ARM_CM3_MPU/port.c
else
TARGET_SRCS += Source/portable/GCC/ARM_CM3/port.c
endif
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
