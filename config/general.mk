-include local.mk

ifeq ($(VERBOSE),)
VERBOSE = false
endif
ifeq ($(DEBUG),)
DEBUG = false
endif
ifeq ($(USE_FLOATINGPOINT),)
USE_FLOATINGPOINT = true
endif

ifneq ($(VERBOSE),true)
MAKE_OPTS = --no-print-directory -s
endif

ifeq ($(DEBUG),true)
TARGET_CPPFLAGS += -DFULLDEBUG
HOST_CPPFLAGS += -DFULLDEBUG
endif
