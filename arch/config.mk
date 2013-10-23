ifeq ($(CPU),arm)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/include

ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx $(ROOTDIR)/arch/arm/lpc17xx/Drivers/include
LDSCRIPT = $(ROOTDIR)/arch/arm/lpc17xx/ldscript
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif

ifeq ($(CPU_FLAVOR),stm32f10)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/stm32f10x/Core/CM3/DeviceSupport/ST/STM32F10x $(ROOTDIR)/arch/arm/stm32f10x/Drivers/include
LDSCRIPT = $(ROOTDIR)/arch/arm/stm32f10x/ldscript
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif


ifeq ($(CPU_FAMILY),CM3)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/src/CM3/CoreSupport
SPECS = $(ROOTDIR)/arch/arm/src/specs
endif

ifeq ($(BOARD),mbed)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/lpc17xx/mbed
endif

ifeq ($(BOARD),inemo)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/stm32f10x/inemo
endif

ifeq ($(BOARD),stm32f4discovery)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/stm32f4/stm32f4discovery
endif

endif

ifeq ($(CPU),mips)
TARGET_INCLUDES += $(ROOTDIR)/arch/mips/include
ifeq ($(CPU_FLAVOR),mips4)
TARGET_INCLUDES += $(ROOTDIR)/arch/mips/mips4
LDSCRIPT = $(ROOTDIR)/arch/mips/mips4/ldscript
SPECS = $(ROOTDIR)/arch/mips/mips4/specs
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif
endif
