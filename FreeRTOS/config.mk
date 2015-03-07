TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/include

TARGET_INCLUDES += $(ROOTDIR)/config/$(CPU_FAMILY)/$(CPU_FLAVOR)

ifeq ($(CPU_FAMILY),arm)
ifeq ($(CPU_SUBFAMILY),CM3)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM3
endif
ifeq ($(CPU_SUBFAMILY),CM4F)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM4F
endif
endif

ifeq ($(CPU_FAMILY),mips)
ifeq ($(CPU_SUBFAMILY),mips4)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/MPLAB/PIC32MX
endif
endif
