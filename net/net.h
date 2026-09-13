#ifndef JAGX_NET_H
#define JAGX_NET_H

#include <stdint.h>

#define NET_IF_LOOPBACK 0

struct net_interface {
    int      id;
    char     name[16];
    uint8_t  mac[6];
    uint32_t ip;
    uint32_t netmask;
    int      up;
};

void net_init(void);
int  net_interface_up(int id);
void net_loopback_send(const void* data, uint32_t len);

#endif
