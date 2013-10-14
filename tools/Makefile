all: romfs cm3-checksum

.PHONY: romfs cm3-checksum

include $(ROOTDIR)/common.mk

romfs:
	$(E) "[MAKE]   Entering romfs"
	$(Q)$(MAKE) $(MAKE_OPTS) -C romfs

cm3-checksum:
	$(E) "[MAKE]   Entering cm3-checksum"
	$(Q)$(MAKE) $(MAKE_OPTS) -C cm3-checksum

clean:
	$(Q)$(MAKE) $(MAKE_OPTS) -C romfs clean
	$(Q)$(MAKE) $(MAKE_OPTS) -C cm3-checksum clean
