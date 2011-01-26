VERBOSE = false

ifneq ($(VERBOSE),true)
MAKE_OPTS = --no-print-directory -s
endif
