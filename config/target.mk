ifeq ($(BOARD),)
export BOARD = mbed
endif
export USE_MPU = true


ifeq ($(BOARD),mbed)
export CPU = arm
export CPU_FLAVOR = lpc1768
endif

ifeq ($(BOARD),pic32)
export CPU = mips
export CPU_FLAVOR = mips4
endif
