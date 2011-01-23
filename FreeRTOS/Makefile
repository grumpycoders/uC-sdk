TARGET_LIB = libFreeRTOS.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk

ifeq ($(USE_MPU),true)
TARGET_CPPFLAGS += -DportUSING_MPU_WRAPPERS=1
endif

TARGET_SRCS = Source/croutine.c Source/list.c Source/queue.c Source/tasks.c
TARGET_INCLUDES = Source/include

ifeq ($(CPU),arm)
ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_SRCS += Source/portable/MemMang/heap_3.c
TARGET_INCLUDES += $(ROOTDIR)/config/arm/lpc1768 $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/CoreSupport
ifeq ($(USE_MPU),true)
TARGET_SRCS += Source/portable/GCC/ARM_CM3_MPU/port.c
TARGET_INCLUDES += Source/portable/GCC/ARM_CM3_MPU
else
TARGET_SRCS += Source/portable/GCC/ARM_CM3/port.c
TARGET_INCLUDES += Source/portable/GCC/ARM_CM3
endif
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
