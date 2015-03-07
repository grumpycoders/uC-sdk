TARGET_LIB = libFreeRTOS.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libc/config.mk

TARGET_SRCS = Source/croutine.c Source/list.c Source/queue.c Source/tasks.c Source/timers.c

TARGET_SRCS += Source/portable/MemMang/heap_3.c

ifeq ($(CPU_FAMILY),arm)
ifeq ($(CPU_SUBFAMILY),CM3)
TARGET_SRCS += Source/portable/GCC/ARM_CM3/port.c
endif
ifeq ($(CPU_SUBFAMILY),CM4F)
TARGET_SRCS += Source/portable/GCC/ARM_CM4F/port.c
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
