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
TARGET_SRCS += arm/src/CM3/handlers.c
endif

ifeq ($(CPU_FAMILY),CM4F)
#TARGET_SRCS += arm/src/CM4F/angel.s
#TARGET_SRCS += arm/src/CM4F/CoreSupport/core_cm4.c
#TARGET_SRCS += arm/src/CM4F/setjmp.s arm/src/CM3/startup.s
#TARGET_SRCS += arm/src/CM4F/handlers.c
endif

ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_SRCS += arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx/system_LPC17xx.c
TARGET_SRCS += $(addprefix arm/lpc17xx/Drivers/source/lpc17xx_, spi.c rit.c exti.c wdt.c uart.c dac.c rtc.c i2s.c pwm.c mcpwm.c pinsel.c nvic.c emac.c systick.c ssp.c can.c gpio.c libcfg_default.c i2c.c timer.c gpdma.c clkpwr.c qei.c adc.c)
TARGET_SRCS += arm/lpc17xx/init.c arm/lpc17xx/Drivers/source/debug_frmwrk.c
TARGET_SRCS += arm/lpc17xx/registry.c
endif

ifeq ($(BOARD),mbed)
TARGET_SRCS += arm/lpc17xx/mbed/BoardConsole.c arm/lpc17xx/mbed/BoardInit.c
endif

ifeq ($(CPU_FLAVOR),stm32f10)
TARGET_SRCS += arm/stm32f10x/Core/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
TARGET_SRCS += $(addprefix arm/stm32f10x/Drivers/source/stm32f10x_, adc.c bkp.c can.c cec.c crc.c dac.c dbgmcu.c dma.c exti.c flash.c fsmc.c gpio.c i2c.c iwdg.c pwr.c rcc.c rtc.c sdio.c spi.c tim.c usart.c wwdg.c)
TARGET_SRCS += arm/stm32f10x/Drivers/source/misc.c
TARGET_SRCS += arm/stm32f10x/init.c
TARGET_SRCS += arm/stm32f10x/registry.c
endif

ifeq ($(CPU_FLAVOR),stm32f4)
TARGET_SRCS += arm/stm32f4xx/Core/CM4F/DeviceSupport/ST/STM32F4xx/system_stm32f4xx.c
TARGET_SRCS += $(addprefix arm/stm32f4xx/Drivers/source/stm32f4xx_, adc.c can.c crc.c cryp_aes.c cryp_des.c cryp_tdes.c cryp.c dac.c dbgmcu.c dcmi.c dma.c exti.c flash.c fsmc.c gpio.c hash_md5.c hash_sha1.c hash.c i2c.c iwdg.c pwr.c rcc.c rng.c rtc.c sdio.c spi.c tim.c usart.c wwdg.c)
TARGET_SRCS += arm/stm32f4xx/Drivers/source/misc.c
TARGET_SRCS += arm/stm32f4xx/init.c
TARGET_SRCS += arm/stm32f4xx/registry.c
endif

ifeq ($(BOARD),inemo)
TARGET_SRCS += arm/stm32f10x/inemo/BoardConsole.c arm/stm32f10x/inemo/BoardInit.c
endif

ifeq ($(BOARD),stm32f4discovery)
TARGET_SRCS += arm/stm32f4xx/stm32f4discovery/BoardConsole.c
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic



