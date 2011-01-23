include $(ROOTDIR)/config/general.mk

ifeq ($(VERBOSE),true)
E = @:
Q =
else
E = @echo
Q = @
endif

include $(ROOTDIR)/config/target.mk
include $(ROOTDIR)/config/toolchain.mk
