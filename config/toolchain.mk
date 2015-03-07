ifeq ($(CPU),arm)
TOOLCHAIN = arm-none-eabi
TARGET_FORMAT = elf32-littlearm
ifeq ($(CPU_FAMILY),CM3)
TARGET_CPPFLAGS += -mcpu=cortex-m3 -mtune=cortex-m3 -D__thumb2__=1 -march=armv7-m -mfix-cortex-m3-ldrd -mthumb -msoft-float
TARGET_LDFLAGS += -mcpu=cortex-m3 -mtune=cortex-m3 -march=armv7-m -mthumb
endif
ifeq ($(CPU_FAMILY),CM4F)
TARGET_CPPFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-math-errno -mtune=cortex-m4 -D__thumb2__=1 -march=armv7e-m -mthumb
TARGET_LDFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-math-errno -mtune=cortex-m4 -march=armv7e-m -mthumb
endif
ifeq ($(CPU_FLAVOR),stm32f10)
TARGET_CPPFLAGS += -DUSE_STDPERIPH_DRIVER
endif
ifeq ($(CPU_FLAVOR),stm32f4)
TARGET_CPPFLAGS += -DUSE_STDPERIPH_DRIVER
endif
ifeq ($(BOARD),inemo)
TARGET_CPPFLAGS += -DSTM32F10X_HD
endif
ifeq ($(BOARD),stm32f4discovery)
TARGET_CPPFLAGS += -DSTM32F40_41xxx
endif
ifeq ($(BOARD),stm32f4discovery2)
TARGET_CPPFLAGS += -DSTM32F429_439xx
endif
TARGET_CPPFLAGS += -Os -mapcs-frame -mno-sched-prolog -fno-hosted -ffunction-sections -fdata-sections -fshort-wchar -D__LITTLE_ENDIAN
TARGET_LDFLAGS += -Wl,--no-wchar-size-warning
endif

ifeq ($(CPU),mips)
TOOLCHAIN = mipsel-elf
TARGET_FORMAT = elf32-littlemips
ifeq ($(CPU_FLAVOR),mips4)
TARGET_CPPFLAGS += 
endif
TARGET_CPPFLAGS += -Os -msoft-float -fno-hosted -ffunction-sections -fdata-sections -fshort-wchar -D__LITTLE_ENDIAN
endif

TARGET_CC = $(TOOLCHAIN)-gcc
TARGET_CXX = $(TOOLCHAIN)-g++
TARGET_LD = $(TOOLCHAIN)-gcc
TARGET_RANLIB = $(TOOLCHAIN)-ranlib
TARGET_AR = $(TOOLCHAIN)-ar
TARGET_AS = $(TOOLCHAIN)-gcc
TARGET_OBJCOPY = $(TOOLCHAIN)-objcopy

TARGET_OBJCOPY_BIN = $(TARGET_OBJCOPY) -I binary -O $(TARGET_FORMAT) --binary-architecture $(CPU)


HOST_CC = gcc
HOST_CXX = g++
HOST_LD = gcc
HOST_RANLIB = ranlib
HOST_AR = ar
HOST_AS = as

HOST_CPPFLAGS += -O3 -g -fno-strict-aliasing

TARGET_CPPFLAGS += -fno-strict-aliasing
