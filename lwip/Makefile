TARGET_LIB = liblwip.a

all: $(TARGET_LIB)

include $(ROOTDIR)/common.mk
include config.mk
include $(ROOTDIR)/FreeRTOS/config.mk
include $(ROOTDIR)/arch/config.mk
include $(ROOTDIR)/os/config.mk
include $(ROOTDIR)/libm/config.mk
include $(ROOTDIR)/libc/config.mk

TARGET_SRCS = \
src/core/def.c \
src/core/mem.c \
src/core/memp.c \
src/core/netif.c \
src/core/pbuf.c \
src/core/raw.c \
src/core/stats.c \
src/core/timers.c \
src/core/sys.c \
src/core/tcp.c \
src/core/tcp_in.c \
src/core/tcp_out.c \
src/core/udp.c \
src/core/dhcp.c \
src/core/dns.c \
src/core/init.c \
\
src/core/ipv4/ip.c \
src/core/ipv4/icmp.c \
src/core/ipv4/inet.c \
src/core/ipv4/ip_addr.c \
src/core/ipv4/ip_frag.c \
src/core/ipv4/inet_chksum.c \
\
src/api/err.c \
\
src/netif/etharp.c \
\
apps/httpserver_raw/httpd.c \
apps/httpserver_raw/fs.c \

ifeq ($(CPU_FAMILY),arm)
ifeq ($(CPU_FLAVOR),lpc17xx)
TARGET_SRCS += src/netif/lpc17xx-if.c
endif
endif

include $(ROOTDIR)/target-rules.mk

clean: clean-generic
