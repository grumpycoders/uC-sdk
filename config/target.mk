ifeq ($(BOARD),)
BOARD = mbed
endif

ifeq ($(BOARD),mbed)
CPU = arm
CPU_FLAVOR = lpc17xx
CPU_FAMILY = CM3

TARGET_CPPFLAGS += -DHAS_ETHERNET -DHAS_SEMIFS -DBOARD_MBED
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

ifeq ($(BOARD),stm32f4discovery)
CPU = arm
CPU_FLAVOR = stm32f4
CPU_FAMILY = CM4F

#TARGET_CPPFLAGS += -DSystemCoreClock=168000000
endif


TARGET_CPPFLAGS += -DPORT_CPU=$(CPU) -DPORT_CPU_FLAVOR=$(CPU_FLAVOR) -DPORT_CPU_FAMILY=$(CPU_FAMILY)
