%.o: %.c
	$(E) "[TC]     Compiling $<"
	$(Q)$(TARGET_CC) -ffunction-sections -Wall -Werror $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -g -c -o $@ $<

%.o: %.cc
	$(E) "[TCXX]   Compiling $<"
	$(Q)$(TARGET_CXX) -ffunction-sections -Wall -Werror $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -g -c -o $@ $<

%.o: %.cpp
	$(E) "[TCXX]   Compiling $<"
	$(Q)$(TARGET_CXX) -ffunction-sections -Wall -Werror $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -g -c -o $@ $<

%.o: %.s
	$(E) "[TS]     Compiling $<"
	$(Q)$(TARGET_AS) $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_ASFLAGS) $(TARGET_CPPFLAGS) -g -c -o $@ $<

%.dep: %.c
	$(Q)$(TARGET_CC) -ffunction-sections -Wall -Werror $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -g -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

%.dep: %.cc
	$(Q)$(TARGET_CXX) -ffunction-sections -Wall -Werror $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -g -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

%.dep: %.cpp
	$(Q)$(TARGET_CXX) -ffunction-sections -Wall -Werror $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -g -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

%.dep: %.s
	$(Q)touch $@

%.dep: %.o
	$(Q)touch $@

TARGET_OBJS += $(addsuffix .o, $(basename $(TARGET_SRCS)))
TARGET_DEPS += $(addsuffix .dep, $(basename $(TARGET_SRCS)))

ifneq ($(TARGET),)
TARGET_ELF = $(addsuffix .elf, $(basename $(TARGET)))
TARGET_BIN = $(addsuffix .bin, $(basename $(TARGET)))
TARGET_MAP = $(addsuffix .map, $(basename $(TARGET)))
TARGET_DEPS += $(addsuffix .dep, $(basename $(TARGET)))
endif

$(TARGET_ELF): $(TARGET_OBJS) $(LIBDEPS) $(LDSCRIPT) $(SPECS)
	$(E) "[TL]     Linking $@"
	$(Q)$(TARGET_LD) -Wl,--gc-sections -Wl,-Map=$(TARGET_MAP) -o $@ $(TARGET_OBJS) $(TARGET_LDFLAGS) -g -T$(LDSCRIPT) -Wl,--entry=_ucsdk_start $(LIBS)

$(TARGET_MAP): $(TARGET_ELF)

$(TARGET_BIN): $(TARGET_ELF)
	$(E) "[TB]     Creating $@"
	$(Q)$(TARGET_OBJCOPY) $< -O binary $@
ifeq ($(CPU_FAMILY),CM3)
	$(Q)$(ROOTDIR)/tools/cm3-checksum/cm3-checksum $@
endif

ifneq ($(TARGET_LIB),)
$(TARGET_LIB): $(TARGET_OBJS)
	$(E) "[TLIB]   Creating $@"
	$(Q)$(TARGET_AR) rcs $@ $^
endif

.PHONY: clean-generic ldeps

clean-generic:
	$(E) "[CLEAN]  $(CURDIR)"
	$(Q)rm -f $(TARGET_LIB) $(TARGET_OBJS) $(TARGET) $(TARGET_ELF) $(TARGET_BIN) $(TARGET_MAP) $(TARGET_DEPS)

ldeps: $(TARGET_DEPS)

-include $(TARGET_DEPS)
