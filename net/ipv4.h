#ifndef JAGX_IPV4_H
#define JAGX_IPV4_H

#include <stdint.h>

uint16_t ip_checksum(const void* data, uint32_t len);
int ipv4_send(uint32_t dst_ip, uint8_t protocol,
              const uint8_t* payload, uint32_t payload_len,
              uint32_t src_ip);

#endif
