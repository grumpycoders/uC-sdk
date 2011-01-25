TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/include

ifeq ($(USE_MPU),true)
TARGET_CPPFLAGS += -DportUSING_MPU_WRAPPERS=1
endif

ifeq ($(CPU),arm)
ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_INCLUDES += $(ROOTDIR)/config/arm/lpc1768
ifeq ($(USE_MPU),true)
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM3_MPU
else
TARGET_INCLUDES += $(ROOTDIR)/FreeRTOS/Source/portable/GCC/ARM_CM3
endif
endif
endif
