export BOARD = mbed
export USE_MPU = false


ifeq ($(BOARD),mbed)
export CPU = arm
export CPU_FLAVOR = lpc1768
endif
