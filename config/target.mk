export BOARD = mbed
export USE_MPU = true


ifeq ($(BOARD),mbed)
export CPU = arm
export CPU_FLAVOR = lpc1768
endif
