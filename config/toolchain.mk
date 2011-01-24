ifeq ($(CPU),arm)
TOOLCHAIN = arm-none-eabi
ifeq ($(CPU_FLAVOR),lpc1768)
TARGET_CPPFLAGS += -mcpu=cortex-m3 -mtune=cortex-m3 -D__thumb2__=1 -march=armv7-m -mfix-cortex-m3-ldrd
endif
TARGET_CPPFLAGS += -mthumb -Os -mapcs-frame -msoft-float -mno-sched-prolog -fno-hosted -ffunction-sections -fdata-sections
endif

TARGET_CC = $(TOOLCHAIN)-gcc
TARGET_CXX = $(TOOLCHAIN)-g++
TARGET_LD = $(TOOLCHAIN)-gcc
TARGET_RANLIB = $(TOOLCHAIN)-ranlib
TARGET_AR = $(TOOLCHAIN)-ar
TARGET_AS = $(TOOLCHAIN)-gcc
TARGET_OBJCOPY = $(TOOLCHAIN)-objcopy
