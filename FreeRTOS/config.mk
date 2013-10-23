TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/include

TARGET_INCLUDES += $(ROOTDIR)/config/$(CPU)/$(CPU_FLAVOR)

ifeq ($(CPU),arm)
ifeq ($(CPU_FAMILY),CM3)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM3
endif
ifeq ($(CPU_FAMILY),CM4F)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM4F
endif
endif

ifeq ($(CPU),mips)
ifeq ($(CPU_FLAVOR),mips4)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/MPLAB/PIC32MX
endif
endif
