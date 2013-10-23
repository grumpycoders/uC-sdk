#pragma once

#include <lwip/netif.h>

err_t interface_init(struct netif *netif);
void interface_check_input(struct netif * netif);
