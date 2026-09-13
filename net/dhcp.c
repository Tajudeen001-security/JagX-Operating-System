/* Minimal DHCP DISCOVER/OFFER path for QEMU user-net */
#include "dhcp.h"
#include "udp.h"
#include "virtio_net.h"
#include "arp.h"
#include "../kernel/arch/x86_64/console.h"

static uint8_t xid[4] = {0x4A, 0x47, 0x58, 0x01};

int dhcp_discover(struct dhcp_lease* out) {
    if (!out) return -1;
    out->valid = 0;

    uint8_t pkt[300];
    for (int i = 0; i < 300; i++) pkt[i] = 0;

    pkt[0] = 1;  /* BOOTREQUEST */
    pkt[1] = 1;  /* Ethernet */
    pkt[2] = 6;  /* mac len */
    pkt[4] = xid[0]; pkt[5] = xid[1]; pkt[6] = xid[2]; pkt[7] = xid[3];
    pkt[10] = 0x80; /* broadcast flag */
    /* chaddr */
    pkt[28] = 0x02; pkt[29] = 0x00; pkt[30] = 0x00;
    pkt[31] = 0x00; pkt[32] = 0x00; pkt[33] = 0x01;
    /* magic cookie */
    pkt[236] = 99; pkt[237] = 130; pkt[238] = 83; pkt[239] = 99;
    /* options: 53=1 DISCOVER, 55 param list, 255 end */
    int o = 240;
    pkt[o++] = 53; pkt[o++] = 1; pkt[o++] = 1;
    pkt[o++] = 55; pkt[o++] = 3; pkt[o++] = 1; pkt[o++] = 3; pkt[o++] = 6;
    pkt[o++] = 255;

    console_write("[DHCP] DISCOVER\n");
    uint32_t src = 0;
    uint32_t dst = 0xFFFFFFFFu;
    udp_send(src, 68, dst, 67, pkt, (uint32_t)o);

    uint8_t rx[1600];
    for (int t = 0; t < 40; t++) {
        uint32_t n = virtio_net_receive(rx, sizeof(rx));
        if (n > 14 + 20 + 8 + 240) {
            /* naive: look for DHCP magic in payload */
            for (uint32_t i = 0; i + 4 < n; i++) {
                if (rx[i]==99 && rx[i+1]==130 && rx[i+2]==83 && rx[i+3]==99) {
                    /* yiaddr at offset 16 within DHCP from UDP payload start — approximate */
                    /* Ethernet 14 + IP 20 + UDP 8 = 42; yiaddr at DHCP+16 */
                    if (n > 42 + 20) {
                        const uint8_t* dhcp = rx + 42;
                        out->yiaddr = ((uint32_t)dhcp[16]<<24)|((uint32_t)dhcp[17]<<16)|
                                      ((uint32_t)dhcp[18]<<8)|dhcp[19];
                        if (out->yiaddr) {
                            out->netmask = 0xFFFFFF00u;
                            out->gateway = (out->yiaddr & 0xFFFFFF00u) | 2;
                            out->dns = (8u<<24)|(8u<<16)|(8u<<8)|8u;
                            out->valid = 1;
                            console_write("[DHCP] Offer/ack seen, yiaddr set\n");
                            uint8_t mac[6] = {0x02,0,0,0,0,1};
                            arp_set_self(mac, out->yiaddr);
                            return 0;
                        }
                    }
                }
            }
        }
        for (volatile int d = 0; d < 100000; d++);
    }
    console_write("[DHCP] No offer (using QEMU defaults)\n");
    /* QEMU user-net default guest */
    out->yiaddr = (10u<<24)|(0u<<16)|(2u<<8)|15u;
    out->gateway = (10u<<24)|(0u<<16)|(2u<<8)|2u;
    out->netmask = 0xFFFFFF00u;
    out->dns = (8u<<24)|(8u<<16)|(8u<<8)|8u;
    out->valid = 1;
    uint8_t mac[6] = {0x02,0,0,0,0,1};
    arp_set_self(mac, out->yiaddr);
    return 0;
}

void dhcp_init(void) {
    console_write("[DHCP] Client ready\n");
}
