ifeq ($(CPU),arm)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/include
ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_INCLUDES += $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/CoreSupport $(ROOTDIR)/arch/arm/lpc17xx/Drivers/include $(ROOTDIR)/arch/arm/lpc17xx/mbed
LDSCRIPT = $(ROOTDIR)/arch/arm/lpc17xx/ldscript
SPECS = $(ROOTDIR)/arch/arm/lpc17xx/specs
endif
endif
