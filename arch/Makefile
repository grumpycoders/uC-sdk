TARGET_LIB = libarch.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libc/config.mk

ifeq ($(CPU),arm)

ifeq ($(CPU_FAMILY),CM3)
TARGET_SRCS += arm/src/CM3/angel.s
TARGET_SRCS += arm/src/CM3/CoreSupport/core_cm3.c
TARGET_SRCS += arm/src/CM3/setjmp.s arm/src/CM3/startup.s
endif

ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_SRCS += arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx/system_LPC17xx.c
TARGET_SRCS += $(addprefix arm/lpc17xx/Drivers/source/lpc17xx_, spi.c rit.c exti.c wdt.c uart.c dac.c rtc.c i2s.c pwm.c mcpwm.c pinsel.c nvic.c emac.c systick.c ssp.c can.c gpio.c libcfg_default.c i2c.c timer.c gpdma.c clkpwr.c qei.c adc.c)
TARGET_SRCS += arm/lpc17xx/deinit-all.c arm/lpc17xx/handlers.c arm/lpc17xx/Drivers/source/debug_frmwrk.c
TARGET_SRCS += arm/lpc17xx/registry.c
endif

ifeq ($(BOARD),mbed)
TARGET_SRCS += arm/lpc17xx/mbed/BoardConsole.c arm/lpc17xx/mbed/BoardInit.c
endif

endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic



