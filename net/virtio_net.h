#ifndef JAGX_VIRTIO_NET_H
#define JAGX_VIRTIO_NET_H

#include <stdint.h>

/* Minimal virtio-net definitions for QEMU */
#define VIRTIO_VENDOR_ID    0x1AF4
#define VIRTIO_NET_DEVICE_ID 0x1000

int  virtio_net_init(void);
void virtio_net_poll(void);

#endif
