ifeq ($(BOARD),mbedlpc1768)
CPU_FAMILY = arm
CPU_SUBFAMILY = CM3
CPU_FLAVOR = lpc17xx
CPU = lpc1768

TARGET_CPPFLAGS += -DHAS_ETHERNET -DHAS_SEMIFS -DBOARD_MBED
endif

ifeq ($(BOARD),inemom1)
CPU_FAMILY = arm
CPU_SUBFAMILY = CM3
CPU_FLAVOR = stm32f10x
CPU = stm32f103
endif

ifeq ($(BOARD),pic32)
CPU_FAMILY = mips
CPU_SUBFAMILY = mips4
endif

ifeq ($(BOARD),stm32f4discovery)
CPU_FAMILY = arm
CPU_SUBFAMILY = CM4F
CPU_FLAVOR = stm32f4xx
CPU = stm32f407

TARGET_CPPFLAGS += -DBOARD_DISCOVERY_F4
#TARGET_CPPFLAGS += -DSystemCoreClock=168000000
endif

ifeq ($(BOARD),stm32f429discovery)
CPU_FAMILY = arm
CPU_SUBFAMILY = CM4F
CPU_FLAVOR = stm32f4xx
CPU = stm32f429

TARGET_CPPFLAGS += -DBOARD_DISCOVERY_F429
#TARGET_CPPFLAGS += -DSystemCoreClock=168000000
endif

ifeq ($(CPU_FAMILY),)
$(info You probably want to compile one of the examples - you can't compile uC-sdk as-it.)
$(info For example, make -C examples/stm32f4discovery/skel)
$(error You need at least a CPU definition.)
endif


TARGET_CPPFLAGS += -DPORT_CPU=$(CPU) -DPORT_CPU_FAMILY=$(CPU_FAMILY) -DPORT_CPU_FAMILY_SUBFAMILY=$(CPU_SUBFAMILY) -DPORT_CPU_FLAVOR=$(CPU_FLAVOR)
