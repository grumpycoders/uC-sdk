#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#include <stdint.h>

extern uint8_t lwip_ram_heap[];

#define IPv6              0
#define NO_SYS            1
#define LWIP_ARP          1
#define LWIP_RAW          1
#define LWIP_UDP          1
#define LWIP_TCP          1
#define LWIP_DNS          1
#define LWIP_DHCP         1
#define LWIP_IGMP         0
#define LWIP_SNMP         0
#define LWIP_SOCKET       0
#define LWIP_NETCONN      0
#define LWIP_AUTOIP       0
#define LWIP_CALLBACK_API 1
#define MEM_LIBC_MALLOC   0
#define MEMP_MEM_MALLOC   0
#define MEM_ALIGNMENT     4
#define MEM_SIZE          5600
#define MEMP_NUM_SYS_TIMEOUT 6

#define LWIP_RAM_HEAP_POINTER lwip_ram_heap

#endif
