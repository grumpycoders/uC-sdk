TARGET_LIB = libarch.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk

ifeq ($(CPU),arm)
ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_INCLUDES += arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx arm/lpc17xx/Core/CM3/CoreSupport arm/lpc17xx/Drivers/include
TARGET_SRCS += arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx/system_LPC17xx.c arm/lpc17xx/Core/CM3/CoreSupport/core_cm3.c
TARGET_SRCS += $(addprefix arm/lpc17xx/Drivers/source/lpc17xx_, spi.c rit.c exti.c wdt.c uart.c dac.c rtc.c i2s.c pwm.c mcpwm.c pinsel.c nvic.c emac.c systick.c ssp.c can.c gpio.c libcfg_default.c i2c.c timer.c gpdma.c clkpwr.c qei.c adc.c)
TARGET_SRCS += arm/lpc17xx/startup.s
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic



