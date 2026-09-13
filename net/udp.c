#include "udp.h"
#include "ipv4.h"
#include "../kernel/arch/x86_64/console.h"

int udp_send(uint32_t src_ip, uint16_t src_port,
             uint32_t dst_ip, uint16_t dst_port,
             const uint8_t* data, uint32_t len) {
    uint8_t packet[1500];
    if (len + 8 > sizeof(packet)) return -1;

    packet[0] = (src_port >> 8) & 0xFF;
    packet[1] = src_port & 0xFF;
    packet[2] = (dst_port >> 8) & 0xFF;
    packet[3] = dst_port & 0xFF;
    uint16_t ulen = (uint16_t)(8 + len);
    packet[4] = (ulen >> 8) & 0xFF;
    packet[5] = ulen & 0xFF;
    packet[6] = 0; packet[7] = 0; /* checksum optional for IPv4 */

    for (uint32_t i = 0; i < len; i++)
        packet[8 + i] = data[i];

    console_write("[UDP] Sending datagram\n");
    return ipv4_send(dst_ip, 17, packet, ulen, src_ip);
}
