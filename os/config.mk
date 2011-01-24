TARGET_INCLUDES += $(ROOTDIR)/os/include

ifeq ($(USE_MPU),true)
TARGET_CPPFLAGS += -DUSING_MPU=1
endif
