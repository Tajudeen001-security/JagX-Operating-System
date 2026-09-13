#ifndef JAGX_VIRTIO_NET_H
#define JAGX_VIRTIO_NET_H

#include <stdint.h>

int  virtio_net_init(void);
void virtio_net_poll(void);
int  virtio_net_transmit(const void* data, uint32_t len);

#endif
