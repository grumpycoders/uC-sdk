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

TARGET_CPPFLAGS += -DBOARD_DISCOVERY_F4
#TARGET_CPPFLAGS += -DSystemCoreClock=168000000
endif

ifeq ($(BOARD),stm32f4discovery2)
CPU = arm
CPU_FLAVOR = stm32f4
CPU_FAMILY = CM4F

TARGET_CPPFLAGS += -DBOARD_DISCOVERY_F4
#TARGET_CPPFLAGS += -DSystemCoreClock=168000000
endif

ifeq ($(CPU),)
$(info You probably want to compile one of the examples - you can't compile uC-sdk as-it.)
$(info For example, make -C examples/stm32f4discovery/skel)
$(error You need at least a CPU definition.)
endif


TARGET_CPPFLAGS += -DPORT_CPU=$(CPU) -DPORT_CPU_FLAVOR=$(CPU_FLAVOR) -DPORT_CPU_FAMILY=$(CPU_FAMILY)
