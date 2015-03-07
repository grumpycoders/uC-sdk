TARGET_LIB = libarch.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libc/config.mk
include $(ROOTDIR)/hardware/config.mk

ifeq ($(CPU_FAMILY),arm)

ifeq ($(CPU_SUBFAMILY),CM3)
TARGET_SRCS += arm/Core/CM3/angel.s
TARGET_SRCS += arm/Core/CM3/CoreSupport/core_cm3.c
TARGET_SRCS += arm/Core/CM3/setjmp.s arm/Core/CM3/startup.s
TARGET_SRCS += arm/Core/CM3/handlers.c
endif

ifeq ($(CPU_SUBFAMILY),CM4F)
TARGET_SRCS += arm/Core/CM4F/angel.s
#TARGET_SRCS += arm/Core/CM4F/CoreSupport/core_cm4.c
TARGET_SRCS += arm/Core/CM4F/startup.s
TARGET_SRCS += arm/Core/CM4F/handlers.c
endif

ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_SRCS += arm/Core/CM3/DeviceSupport/NXP/lpc17xx/system_LPC17xx.c
TARGET_SRCS += $(addprefix arm/lpc17xx/Drivers/source/lpc17xx_, spi.c rit.c exti.c wdt.c uart.c dac.c rtc.c i2s.c pwm.c mcpwm.c pinsel.c nvic.c emac.c systick.c ssp.c can.c gpio.c libcfg_default.c i2c.c timer.c gpdma.c clkpwr.c qei.c adc.c)
TARGET_SRCS += arm/lpc17xx/init.c arm/lpc17xx/Drivers/source/debug_frmwrk.c
TARGET_SRCS += arm/lpc17xx/registry.c
endif

ifeq ($(BOARD),mbed-lpc1768)
TARGET_SRCS += arm/lpc17xx/mbed-lpc1768/BoardConsole.c arm/lpc17xx/mbed-lpc1768/BoardInit.c
endif

ifeq ($(CPU_FLAVOR),stm32f10x)
TARGET_SRCS += arm/Core/CM3/DeviceSupport/ST/stm32f10x/system_stm32f10x.c
TARGET_SRCS += $(addprefix arm/stm32f10x/Drivers/source/stm32f10x_, adc.c bkp.c can.c cec.c crc.c dac.c dbgmcu.c dma.c exti.c flash.c fsmc.c gpio.c i2c.c iwdg.c pwr.c rcc.c rtc.c sdio.c spi.c tim.c usart.c wwdg.c)
TARGET_SRCS += arm/stm32f10x/Drivers/source/misc.c
TARGET_SRCS += arm/stm32f10x/init.c
TARGET_SRCS += arm/stm32f10x/registry.c
endif

ifeq ($(CPU_FLAVOR),stm32f4xx)
TARGET_SRCS += arm/Core/CM4F/DeviceSupport/ST/stm32f4xx/system_stm32f4xx.c
TARGET_SRCS += $(addprefix arm/stm32f4xx/Drivers/source/stm32f4xx_, adc.c can.c crc.c cryp_aes.c cryp_des.c cryp_tdes.c cryp.c dac.c dbgmcu.c dcmi.c dma.c dma2d.c exti.c flash.c gpio.c hash_md5.c hash_sha1.c hash.c i2c.c iwdg.c ltdc.c pwr.c rcc.c rng.c rtc.c sai.c sdio.c spi.c syscfg.c tim.c usart.c wwdg.c)
#STM32F427_437xx, STM32F429_439xx: fmc.c 
#STM32F40_41xxx: fsmc.c
TARGET_SRCS += arm/stm32f4xx/Drivers/source/misc.c
TARGET_SRCS += arm/stm32f4xx/init.c
TARGET_SRCS += arm/stm32f4xx/registry.c
endif

ifeq ($(BOARD),inemom1)
TARGET_SRCS += arm/stm32f10x/inemom1/BoardConsole.c arm/stm32f10x/inemom1/BoardInit.c
endif

ifeq ($(BOARD),stm32f4discovery)
TARGET_SRCS += $(addprefix arm/stm32f4xx/Drivers/source/stm32f4xx_, fsmc.c)
TARGET_SRCS += arm/stm32f4xx/stm32f4discovery/BoardConsole.c arm/stm32f4xx/stm32f4discovery/BoardInit.c
endif

ifeq ($(BOARD),stm32f429discovery)
TARGET_SRCS += $(addprefix arm/stm32f4xx/Drivers/source/stm32f4xx_, fmc.c)
TARGET_SRCS += arm/stm32f4xx/stm32f429discovery/BoardConsole.c arm/stm32f4xx/stm32f429discovery/BoardInit.c
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic



