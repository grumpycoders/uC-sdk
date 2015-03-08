TARGET_INCLUDES += $(ROOTDIR)/hardware/include


ifeq ($(CPU_SUBFAMILY),CM4F)
ifeq ($(CPU_FLAVOR),stm32f4xx)
TARGET_INCLUDES += $(ROOTDIR)/hardware/src/stm32f4xx
endif
endif
