ifeq ($(CPU),arm)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/include

ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx $(ROOTDIR)/arch/arm/lpc17xx/Drivers/include $(ROOTDIR)/arch/arm/lpc17xx/mbed
LDSCRIPT = $(ROOTDIR)/arch/arm/lpc17xx/ldscript
TARGET_CPPFLAGS += -DTARGET_LITTLE_ENDIAN
CPU_FAMILY = CM3
endif

ifeq ($(CPU_FAMILY),CM3)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/src/CM3/CoreSupport
SPECS = $(ROOTDIR)/arch/arm/src/specs
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
