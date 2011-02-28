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

#define MEMP_NUM_SYS_TIMEOUT    6

#define ARP_QUEUING             0
#define ARP_TABLE_SIZE          4
#define DNS_TABLE_SIZE          4
#define DNS_USES_STATIC_BUF     0
#define IP_FRAG_USES_STATIC_BUF 0
#define LWIP_STATS              0
#define TCP_SND_BUF             2048
#define TCP_SND_QUEUELEN        16

#define MEMP_NUM_TCP_PCB        5
#define MEMP_NUM_TCP_PCB_LISTEN 8
#define MEMP_NUM_TCP_SEG        20
#define MEMP_NUM_PBUF           16
#define PBUF_POOL_SIZE          6

#define LWIP_RAM_HEAP_POINTER lwip_ram_heap

#define LWIP_DEBUG

#define LWIP_HTTPD_DYNAMIC_HEADERS 1
#define LWIP_HTTPD_CGI 1

#define ETHARP_DEBUG                    LWIP_DBG_OFF
#define NETIF_DEBUG                     LWIP_DBG_OFF
#define PBUF_DEBUG                      LWIP_DBG_OFF
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define IGMP_DEBUG                      LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       LWIP_DBG_OFF
#define MEMP_DEBUG                      LWIP_DBG_OFF
#define SYS_DEBUG                       LWIP_DBG_OFF
#define TIMERS_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define PPP_DEBUG                       LWIP_DBG_OFF
#define SLIP_DEBUG                      LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_OFF
#define AUTOIP_DEBUG                    LWIP_DBG_OFF
#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
#define DNS_DEBUG                       LWIP_DBG_OFF
#define HTTPD_DEBUG                     LWIP_DBG_OFF
#define HTTPD_DEBUG_TIMING              LWIP_DBG_OFF

#endif
