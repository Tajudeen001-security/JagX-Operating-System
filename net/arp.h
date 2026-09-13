#ifndef JAGX_ARP_H
#define JAGX_ARP_H

#include <stdint.h>

void arp_init(void);
void arp_set_self(const uint8_t mac[6], uint32_t ip);
int  arp_request(uint32_t target_ip);
int  arp_lookup(uint32_t ip, uint8_t out_mac[6]);
void arp_input(const uint8_t* frame, uint32_t len);

#endif
