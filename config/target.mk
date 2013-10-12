ifeq ($(BOARD),)
BOARD = mbed
endif

ifeq ($(USE_MPU),)
USE_MPU = true
endif


ifeq ($(BOARD),mbed)
CPU = arm
CPU_FLAVOR = lpc1768
CPU_FAMILY = CM3
endif

ifeq ($(BOARD),inemo)
CPU = arm
CPU_FLAVOR = stm32f10
CPU_FAMILY = CM3
endif

ifeq ($(BOARD),pic32)
CPU = mips
CPU_FLAVOR = mips4
endif
