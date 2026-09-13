#ifndef JAGX_DHCP_H
#define JAGX_DHCP_H

#include <stdint.h>

struct dhcp_lease {
    uint32_t yiaddr;
    uint32_t gateway;
    uint32_t netmask;
    uint32_t dns;
    int      valid;
};

void dhcp_init(void);
int  dhcp_discover(struct dhcp_lease* out);

#endif
