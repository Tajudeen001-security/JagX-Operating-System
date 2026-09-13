#include "ipv4.h"
#include "virtio_net.h"
#include "../kernel/arch/x86_64/console.h"

uint16_t ip_checksum(const void* data, uint32_t len) {
    const uint16_t* p = (const uint16_t*)data;
    uint32_t sum = 0;
    while (len > 1) {
        sum += *p++;
        len -= 2;
    }
    if (len) sum += *(const uint8_t*)p;
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum);
}

int ipv4_send(uint32_t dst_ip, uint8_t protocol,
              const uint8_t* payload, uint32_t payload_len,
              uint32_t src_ip) {
    uint8_t frame[1514];
    uint32_t pos = 0;

    /* Ethernet header: dest FF:FF:FF:FF:FF:FF (broadcast for early bring-up),
       src 02:00:00:00:00:01, type IPv4 */
    for (int i = 0; i < 6; i++) frame[pos++] = 0xFF;
    frame[pos++] = 0x02; frame[pos++] = 0x00; frame[pos++] = 0x00;
    frame[pos++] = 0x00; frame[pos++] = 0x00; frame[pos++] = 0x01;
    frame[pos++] = 0x08; frame[pos++] = 0x00;

    uint32_t ip_start = pos;
    frame[pos++] = 0x45;                     /* Version 4, IHL 5 */
    frame[pos++] = 0x00;                     /* DSCP */
    uint16_t total = (uint16_t)(20 + payload_len);
    frame[pos++] = (total >> 8) & 0xFF;
    frame[pos++] = total & 0xFF;
    frame[pos++] = 0x00; frame[pos++] = 0x01; /* ID */
    frame[pos++] = 0x40; frame[pos++] = 0x00; /* Don't fragment */
    frame[pos++] = 64;                       /* TTL */
    frame[pos++] = protocol;
    frame[pos++] = 0; frame[pos++] = 0;      /* checksum placeholder */

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

    int rc = virtio_net_transmit(frame, pos);
    if (rc == 0)
        console_write("[IPv4] Frame handed to virtio TX\n");
    return rc;
}
