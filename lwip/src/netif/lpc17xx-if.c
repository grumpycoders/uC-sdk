#include "lwip/opt.h"

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"
#include "netif/lpc17xx-if.h"

#include <lpc17xx_emac.h>
#include <lpc17xx_nvic.h>
#include <lpc17xx_pinsel.h>

#define IFNAME0 'e'
#define IFNAME1 'x'

__attribute__((section(".hwregistry"))) uint8_t mac_address[6] = { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };

/* Forward declarations. */
static void  lpc17xx_if_input(struct netif *netif);
static void  wait_DMA_slot();

/**
 * In this function, the hardware should be initialized.
 * Called from lpc17xx_if_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this lpc17xx_if
 */
static int
low_level_init(struct netif *netif)
{
  EMAC_CFG_Type EMAC_Config;
  PINSEL_CFG_Type PinCfg;
  uint32_t delay, retry;
  
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  memcpy(netif->hwaddr, mac_address, 6);

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  /* initializing lpc17xx's emac. */
  PinCfg.Funcnum = 1;
  PinCfg.OpenDrain = 0;
  PinCfg.Pinmode = 0;
  PinCfg.Portnum = 1;

  PinCfg.Pinnum = 0;  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 1;  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 4;  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 8;  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 9;  PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 10; PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 14; PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 15; PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 16; PINSEL_ConfigPin(&PinCfg);
  PinCfg.Pinnum = 17; PINSEL_ConfigPin(&PinCfg);
  EMAC_Config.Mode = EMAC_MODE_AUTO;
  EMAC_Config.pbEMAC_Addr = netif->hwaddr;
  for (retry = 0; retry < 500; retry++) {
    if (EMAC_Init(&EMAC_Config) != ERROR)
      return 1;
    for(delay = 0x100000; delay; delay--);
  }
  return 0;
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this lpc17xx_if
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  EMAC_PACKETBUF_Type TxPack;

#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  if (EMAC_CheckTransmitIndex() == FALSE) {
    wait_DMA_slot();
  }

  for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    TxPack.ulDataLen = q->len;
    TxPack.pbDataBuf = q->payload;
    EMAC_WritePacketBuffer(&TxPack, FALSE);
  }
  TxPack.ulDataLen = 0;
  TxPack.pbDataBuf = NULL;
  EMAC_WritePacketBuffer(&TxPack, TRUE);
  EMAC_UpdateTxProduceIndex();

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  
  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this lpc17xx_if
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len;

  if (EMAC_CheckReceiveIndex() == FALSE)
    return NULL;

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  len = EMAC_GetReceiveDataSize() + 1;

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
    for(q = p; q != NULL; q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable.
       * This does not necessarily have to be a memcpy, you can also preallocate
       * pbufs for a DMA-enabled MAC and after receiving truncate it to the
       * actually received size. In this case, ensure the tot_len member of the
       * pbuf is the sum of the chained pbuf len members.
       */
      EMAC_PACKETBUF_Type RxPack;
      RxPack.pbDataBuf = q->payload;
      RxPack.ulDataLen = q->len;
      EMAC_ReadPacketBuffer(&RxPack);
    }

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  } else {
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }
  EMAC_UpdateRxConsumeIndex();

  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this lpc17xx_if
 */
static void
lpc17xx_if_input(struct netif *netif)
{
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("lpc17xx_if_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this lpc17xx_if
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
static int lpc17xx_if_got_init = 0;

err_t
lpc17xx_if_init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));
  /* We only have one EMAC on the lpc17xx, so... */
  if (!__sync_bool_compare_and_swap(&lpc17xx_if_got_init, 0, 1))
    return ERR_VAL;

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 746);

  netif->state = NULL;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  
  /* initialize the hardware */
  if (!low_level_init(netif)) {
    lpc17xx_if_got_init = 0;
    return ERR_IF;
  }

  return ERR_OK;
}

lpc17xx_if_waitdma_t lpc17xx_if_waitdma = NULL;

static void wait_DMA_slot(void) {
    if (lpc17xx_if_waitdma)
        lpc17xx_if_waitdma();
    while (EMAC_CheckTransmitIndex() == FALSE);
}

void lpc17xx_if_check_input(struct netif * netif) {
    lpc17xx_if_input(netif);
}

__attribute__((section(".eth_ram"))) uint8_t lwip_ram_heap[5632];
