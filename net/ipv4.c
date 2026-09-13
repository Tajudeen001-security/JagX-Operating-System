#include "ipv4.h"
#include "virtio_net.h"
#include "arp.h"
#include "../kernel/arch/x86_64/console.h"

uint16_t ip_checksum(const void* data, uint32_t len) {
    const uint16_t* p = (const uint16_t*)data;
    uint32_t sum = 0;
    while (len > 1) { sum += *p++; len -= 2; }
    if (len) sum += *(const uint8_t*)p;
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum);
}

int ipv4_send(uint32_t dst_ip, uint8_t protocol,
              const uint8_t* payload, uint32_t payload_len,
              uint32_t src_ip) {
    uint8_t frame[1514];
    uint32_t pos = 0;

    uint8_t dst_mac[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    if (arp_lookup(dst_ip, dst_mac) != 0) {
        /* try resolve gateway-style: request then broadcast */
        arp_request(dst_ip);
        /* brief poll for reply */
        uint8_t rx[256];
        for (int i = 0; i < 10; i++) {
            uint32_t n = virtio_net_receive(rx, sizeof(rx));
            if (n) arp_input(rx, n);
            if (arp_lookup(dst_ip, dst_mac) == 0) break;
            for (volatile int d = 0; d < 50000; d++);
        }
    }

    for (int i = 0; i < 6; i++) frame[pos++] = dst_mac[i];
    frame[pos++] = 0x02; frame[pos++] = 0x00; frame[pos++] = 0x00;
    frame[pos++] = 0x00; frame[pos++] = 0x00; frame[pos++] = 0x01;
    frame[pos++] = 0x08; frame[pos++] = 0x00;

    uint32_t ip_start = pos;
    frame[pos++] = 0x45;
    frame[pos++] = 0x00;
    uint16_t total = (uint16_t)(20 + payload_len);
    frame[pos++] = (total >> 8) & 0xFF;
    frame[pos++] = total & 0xFF;
    frame[pos++] = 0x00; frame[pos++] = 0x01;
    frame[pos++] = 0x40; frame[pos++] = 0x00;
    frame[pos++] = 64;
    frame[pos++] = protocol;
    frame[pos++] = 0; frame[pos++] = 0;
    frame[pos++] = (src_ip >> 24) & 0xFF;
    frame[pos++] = (src_ip >> 16) & 0xFF;
    frame[pos++] = (src_ip >> 8) & 0xFF;
    frame[pos++] = src_ip & 0xFF;
    frame[pos++] = (dst_ip >> 24) & 0xFF;
    frame[pos++] = (dst_ip >> 16) & 0xFF;
    frame[pos++] = (dst_ip >> 8) & 0xFF;
    frame[pos++] = dst_ip & 0xFF;

    uint16_t csum = ip_checksum(frame + ip_start, 20);
    frame[ip_start + 10] = (csum >> 8) & 0xFF;
    frame[ip_start + 11] = csum & 0xFF;

    for (uint32_t i = 0; i < payload_len; i++)
        frame[pos++] = payload[i];

    return virtio_net_transmit(frame, pos);
}
