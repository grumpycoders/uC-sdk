ifeq ($(DEBUG),true)
TARGET_CPPFLAGS += -g -O0 -D_DEBUG
else
TARGET_CPPFLAGS += -g -Os -DNDEBUG
endif

ifeq ($(CPU_FAMILY),arm)
TOOLCHAIN = arm-none-eabi
TARGET_FORMAT = elf32-littlearm
ifeq ($(CPU_SUBFAMILY),CM3)
TARGET_CPPFLAGS += -mcpu=cortex-m3 -mtune=cortex-m3 -D__thumb2__=1 -march=armv7-m -mfix-cortex-m3-ldrd -mthumb -msoft-float
TARGET_LDFLAGS += -mcpu=cortex-m3 -mtune=cortex-m3 -march=armv7-m -mthumb
endif
ifeq ($(CPU_SUBFAMILY),CM4F)
TARGET_CPPFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-math-errno -mtune=cortex-m4 -D__thumb2__=1 -march=armv7e-m -mthumb
TARGET_LDFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-math-errno -mtune=cortex-m4 -march=armv7e-m -mthumb
endif
ifeq ($(CPU_FLAVOR),stm32f10x)
TARGET_CPPFLAGS += -DUSE_STDPERIPH_DRIVER
endif
ifeq ($(CPU_FLAVOR),stm32f4xx)
TARGET_CPPFLAGS += -DUSE_STDPERIPH_DRIVER
endif
ifeq ($(CPU),stm32f103)
TARGET_CPPFLAGS += -DSTM32F10X_HD
endif
ifeq ($(CPU),stm32f407)
TARGET_CPPFLAGS += -DSTM32F40_41xxx
endif
ifeq ($(CPU),stm32f429)
TARGET_CPPFLAGS += -DSTM32F429_439xx
endif
TARGET_CPPFLAGS += -mapcs-frame -mno-sched-prolog -fshort-wchar -D__LITTLE_ENDIAN
TARGET_LDFLAGS += -Wl,--no-wchar-size-warning
endif

ifeq ($(CPU_FAMILY),mips)
TOOLCHAIN = mipsel-elf
TARGET_FORMAT = elf32-littlemips
ifeq ($(CPU_SUBFAMILY),mips4)
TARGET_CPPFLAGS +=
endif
TARGET_CPPFLAGS += -msoft-float -fno-hosted -fshort-wchar -D__LITTLE_ENDIAN
endif


TARGET_CPPFLAGS += -fno-strict-aliasing -ffunction-sections -fdata-sections -fno-builtin-malloc -fno-builtin-realloc -fno-builtin-free -fno-builtin-memset -fno-builtin-memcpy -fno-builtin-memcmp

TARGET_CFLAGS += -std=c99

TARGET_CXXFLAGS += -fno-exceptions -fno-rtti -std=c++11

TARGET_LDFLAGS += -nodefaultlibs -nostartfiles -lgcc

TARGET_CC = $(TOOLCHAIN)-gcc
TARGET_CXX = $(TOOLCHAIN)-g++
TARGET_LD = $(TOOLCHAIN)-gcc
TARGET_RANLIB = $(TOOLCHAIN)-ranlib
TARGET_AR = $(TOOLCHAIN)-ar
TARGET_AS = $(TOOLCHAIN)-gcc
TARGET_OBJCOPY = $(TOOLCHAIN)-objcopy

TARGET_OBJCOPY_BIN = $(TARGET_OBJCOPY) -I binary -O $(TARGET_FORMAT) --binary-architecture $(CPU_FAMILY)


HOST_CC = gcc
HOST_CXX = g++
HOST_LD = gcc
HOST_RANLIB = ranlib
HOST_AR = ar
HOST_AS = as

HOST_CPPFLAGS += -O3 -g -fno-strict-aliasing
