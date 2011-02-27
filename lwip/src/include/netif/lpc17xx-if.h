#ifndef __LPC17XX_IF_H__
#define __LPC17XX_IF_H__

#include "lwip/netif.h"

typedef void (*lpc17xx_if_waitdma_t)();
extern lpc17xx_if_waitdma_t lpc17xX_if_waitdma;

err_t lpc17xx_if_init(struct netif * netif);
void lpc17xx_if_check_input(struct netif * netif);

#endif
