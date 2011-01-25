%.o: %.c
	$(E) "[TC]     Compiling $<"
	$(Q)$(TARGET_CC) $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CPPFLAGS) -c -o $@ $<

%.o: %.s
	$(E) "[TS]     Compiling $<"
	$(Q)$(TARGET_AS) $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CPPFLAGS) -c -o $@ $<

TARGET_OBJS = $(addsuffix .o, $(basename $(TARGET_SRCS)))

ifneq ($(TARGET),)
TARGET_ELF = $(addsuffix .elf, $(basename $(TARGET)))
TARGET_BIN = $(addsuffix .bin, $(basename $(TARGET)))
TARGET_MAP = $(addsuffix .map, $(basename $(TARGET)))
TARGET_OBJS += $(addsuffix .o, $(basename $(TARGET)))
endif

$(TARGET_ELF): $(TARGET_OBJS) $(LIBDEPS)
	$(E) "[TL]     Linking $@"
	$(Q)$(TARGET_LD) -Wl,--gc-sections -Wl,-O3 -Wl,-Map=$(TARGET_MAP) -o $@ $^ -T$(LDSCRIPT) $(LIBS)

$(TARGET_MAP): $(TARGET_ELF)

$(TARGET_BIN): $(TARGET_ELF)
	$(E) "[TB]     Creating $@"
	$(Q)$(TARGET_OBJCOPY) $< -O binary $@

$(TARGET_LIB): $(TARGET_OBJS)
	$(E) "[TLIB]   Creating $@"
	$(Q)$(TARGET_AR) rcs $@ $^

.PHONY: clean-generic

clean-generic:
	$(E) "[CLEAN]  $(CURDIR)"
	$(Q)rm -f $(TARGET_LIB) $(TARGET_OBJS) $(TARGET) $(TARGET_ELF) $(TARGET_BIN) $(TARGET_MAP)
