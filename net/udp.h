#ifndef JAGX_UDP_H
#define JAGX_UDP_H

#include <stdint.h>

int udp_send(uint32_t src_ip, uint16_t src_port,
             uint32_t dst_ip, uint16_t dst_port,
             const uint8_t* data, uint32_t len);

#endif
