%.o: %.c
	$(E) "[C]      Compiling $<"
	$(Q)$(HOST_CC) -Wall -Werror $(addprefix -I, $(HOST_INCLUDES)) $(HOST_CFLAGS) $(HOST_CPPFLAGS) -g -c -o $@ $<

%.o: %.cc
	$(E) "[CXX]    Compiling $<"
	$(Q)$(HOST_CXX) -Wall -Werror $(addprefix -I, $(HOST_INCLUDES)) $(HOST_CFLAGS) $(HOST_CPPFLAGS) -g -c -o $@ $<

%.o: %.cpp
	$(E) "[CXX]    Compiling $<"
	$(Q)$(HOST_CXX) -Wall -Werror $(addprefix -I, $(HOST_INCLUDES)) $(HOST_CFLAGS) $(HOST_CPPFLAGS) -g -c -o $@ $<

%.o: %.s
	$(E) "[S]      Compiling $<"
	$(Q)$(HOST_AS) $(addprefix -I, $(HOST_INCLUDES)) $(HOST_ASFLAGS) $(HOST_CPPFLAGS) -g -c -o $@ $<

%.dep: %.c
	$(Q)$(HOST_CC) -Wall -Werror $(addprefix -I, $(HOST_INCLUDES)) $(HOST_CFLAGS) $(HOST_CPPFLAGS) -g -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

%.dep: %.cc
	$(Q)$(HOST_CXX) -Wall -Werror $(addprefix -I, $(HOST_INCLUDES)) $(HOST_CFLAGS) $(HOST_CPPFLAGS) -g -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

%.dep: %.cpp
	$(Q)$(HOST_CXX) -Wall -Werror $(addprefix -I, $(HOST_INCLUDES)) $(HOST_CFLAGS) $(HOST_CPPFLAGS) -g -M -MT $(addsuffix .o, $(basename $@)) -MF $@ $<

%.dep: %.s
	$(Q)touch $@

%.dep: %.o
	$(Q)touch $@

TARGET_OBJS += $(addsuffix .o, $(basename $(TARGET_SRCS)))
TARGET_DEPS += $(addsuffix .dep, $(basename $(TARGET_SRCS)))

ifneq ($(TARGET),)
TARGET_OBJS += $(addsuffix .o, $(basename $(TARGET)))
TARGET_DEPS += $(addsuffix .dep, $(basename $(TARGET)))
endif

$(TARGET): $(TARGET_OBJS) $(LIBDEPS)
	$(E) "[L]      Linking $@"
	$(Q)$(HOST_LD) -o $@ $(TARGET_OBJS) $(HOST_LDFLAGS) -g $(LIBS)

$(TARGET_LIB): $(TARGET_OBJS)
	$(E) "[LIB]    Creating $@"
	$(Q)$(HOST_AR) rcs $@ $^

.PHONY: clean-generic ldeps

clean-generic:
	$(E) "[CLEAN]  $(CURDIR)"
	$(Q)rm -f $(TARGET_LIB) $(TARGET_OBJS) $(TARGET) $(TARGET_MAP) $(TARGET_DEPS)

ldeps: $(TARGET_DEPS)

-include $(TARGET_DEPS)
