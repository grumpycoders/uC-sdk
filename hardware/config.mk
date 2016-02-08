TARGET_INCLUDES += $(ROOTDIR)/hardware/include


ifeq ($(CPU_SUBFAMILY),CM4F)
ifeq ($(CPU_FLAVOR),stm32f4xx)
TARGET_INCLUDES += $(ROOTDIR)/hardware/src/stm32f4xx
endif
endif

ifeq ($(CPU_SUBFAMILY),CM3)
ifeq ($(CPU_FLAVOR),stm32f10x)
TARGET_INCLUDES += $(ROOTDIR)/hardware/src/stm32f10x
endif
endif
