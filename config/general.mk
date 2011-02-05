VERBOSE = false
DEBUG = true
USE_FLOATINGPOINT = true

ifneq ($(VERBOSE),true)
MAKE_OPTS = --no-print-directory -s
endif

ifeq ($(DEBUG),true)
TARGET_CPPFLAGS += -DFULLDEBUG
endif