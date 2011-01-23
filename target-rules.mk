%.o: %.c
	$(E) [TC]     Compiling $<
	$(Q)$(TARGET_CC) $(addprefix -I, $(TARGET_INCLUDES)) $(TARGET_CPPFLAGS) -c -o $@ $<

TARGET_OBJS = $(addsuffix .o, $(basename $(TARGET_SRCS)))

$(TARGET_LIB): $(TARGET_OBJS)
	$(E) [TLIB]   Creating $@
	$(Q)$(TARGET_AR) rcs $@ $^

.PHONY: clean-generic

clean-generic:
	rm -f $(TARGET_LIB) $(TARGET_OBJS)
