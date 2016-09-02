TARGET_LIB = libhardware.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include $(ROOTDIR)/arch/config.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/libc/config.mk

ifeq ($(CPU_SUBFAMILY),CM3)
ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_SRCS += src/lpc17xx/gpio.c
TARGET_SRCS += src/lpc17xx/ssp.c
endif

ifeq ($(CPU_FLAVOR),stm32f10x)
TARGET_SRCS += src/stm32f10x/adc.c
TARGET_SRCS += src/stm32f10x/gpio.c
TARGET_SRCS += src/stm32f10x/ssp.c
TARGET_SRCS += src/stm32f10x/i2c.c
TARGET_SRCS += src/stm32f10x/timer.c
TARGET_SRCS += src/stm32f10x/uart.c
endif
endif

ifeq ($(CPU_SUBFAMILY),CM4F)
ifeq ($(CPU_FLAVOR),stm32f4xx)
TARGET_SRCS += src/stm32f4xx/adc.c
TARGET_SRCS += src/stm32f4xx/gpio.c
TARGET_SRCS += src/stm32f4xx/i2c.c
TARGET_SRCS += src/stm32f4xx/ssp.c
TARGET_SRCS += src/stm32f4xx/timer.c
TARGET_SRCS += src/stm32f4xx/uart.c
TARGET_SRCS += src/stm32f4xx/irq.c
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
