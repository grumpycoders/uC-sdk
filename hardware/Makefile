TARGET_LIB = libhardware.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/libc/config.mk

TARGET_SRCS = \
src/sdcard.c \

ifeq ($(CPU_FAMILY),CM3)
ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_SRCS += src/gpio-lpc17xx.c
TARGET_SRCS += src/ssp-lpc17xx.c
endif
endif

ifeq ($(CPU_FAMILY),CM4F)
ifeq ($(CPU_FLAVOR),stm32f4)
#TARGET_SRCS += src/adc-stm32f4xx.c
TARGET_SRCS += src/gpio-stm32f4xx.c
#TARGET_SRCS += src/i2c-stm32f4xx.c
TARGET_SRCS += src/timer-stm32f4xx.c
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
