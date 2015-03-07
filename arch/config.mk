ifeq ($(CPU_FAMILY),arm)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/include

ifeq ($(CPU_SUBFAMILY),CM3)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/Core/CM3/CoreSupport
SPECS = $(ROOTDIR)/arch/arm/src/specs
endif

ifeq ($(CPU_SUBFAMILY),CM4F)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/Core/CM4F/CoreSupport
SPECS = $(ROOTDIR)/arch/arm/src/specs
endif

ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/Core/CM3/DeviceSupport/NXP/lpc17xx $(ROOTDIR)/arch/arm/lpc17xx/Drivers/include
LDSCRIPT = $(ROOTDIR)/arch/arm/lpc17xx/ldscript
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif

ifeq ($(CPU_FLAVOR),stm32f10x)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/Core/CM3/DeviceSupport/ST/stm32f10x $(ROOTDIR)/arch/arm/stm32f10x/Drivers/include
LDSCRIPT = $(ROOTDIR)/arch/arm/stm32f10x/ldscript
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif

ifeq ($(CPU_FLAVOR),stm32f4xx)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/Core/CM4F/DeviceSupport/ST/stm32f4xx $(ROOTDIR)/arch/arm/stm32f4xx/Drivers/include
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif

ifeq ($(BOARD),mbed-lpc1768)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/lpc17xx/mbed-lpc1768
endif

ifeq ($(BOARD),inemom1)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/stm32f10x/inemom1
endif

ifeq ($(BOARD),stm32f4discovery)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/stm32f4xx/stm32f4discovery
TARGET_CPPFLAGS += -DHSE_VALUE=8000000
TARGET_CPPFLAGS += -DPLL_M=8
LDSCRIPT = $(ROOTDIR)/arch/arm/stm32f4xx/stm32f40x.ld
endif

ifeq ($(BOARD),stm32f429discovery)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/stm32f4xx/stm32f429discovery
TARGET_CPPFLAGS += -DHSE_VALUE=8000000
TARGET_CPPFLAGS += -DPLL_M=8
LDSCRIPT = $(ROOTDIR)/arch/arm/stm32f4xx/stm32f429.ld
endif

endif

ifeq ($(CPU_FAMILY),mips)
TARGET_INCLUDES += $(ROOTDIR)/arch/mips/include
ifeq ($(CPU_SUBFAMILY),mips4)
TARGET_INCLUDES += $(ROOTDIR)/arch/mips/mips4
LDSCRIPT = $(ROOTDIR)/arch/mips/mips4/ldscript
SPECS = $(ROOTDIR)/arch/mips/mips4/specs
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
endif
endif
