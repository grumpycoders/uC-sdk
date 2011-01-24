TARGET_INCLUDES = $(ROOTDIR)/FreeRTOS/Source/include

ifeq ($(CPU),arm)
ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_INCLUDES += $(ROOTDIR)/config/arm/lpc1768 $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/DeviceSupport/NXP/LPC17xx $(ROOTDIR)/arch/arm/lpc17xx/Core/CM3/CoreSupport
ifeq ($(USE_MPU),true)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM3_MPU
else
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM3
endif
endif
endif
