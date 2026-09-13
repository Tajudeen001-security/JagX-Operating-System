#include "arp.h"
#include "virtio_net.h"
#include "../kernel/arch/x86_64/console.h"

#define ARP_TABLE 16

struct arp_entry {
    uint32_t ip;
    uint8_t  mac[6];
    int      valid;
};

static struct arp_entry table[ARP_TABLE];
static uint8_t self_mac[6] = {0x02,0x00,0x00,0x00,0x00,0x01};
static uint32_t self_ip = 0;

void arp_init(void) {
    for (int i = 0; i < ARP_TABLE; i++) table[i].valid = 0;
    console_write("[ARP] Table ready\n");
}

void arp_set_self(const uint8_t mac[6], uint32_t ip) {
    for (int i = 0; i < 6; i++) self_mac[i] = mac[i];
    self_ip = ip;
}

int arp_lookup(uint32_t ip, uint8_t out_mac[6]) {
    for (int i = 0; i < ARP_TABLE; i++) {
        if (table[i].valid && table[i].ip == ip) {
            for (int j = 0; j < 6; j++) out_mac[j] = table[i].mac[j];
            return 0;
        }
    }
    return -1;
}

static void arp_learn(uint32_t ip, const uint8_t mac[6]) {
    for (int i = 0; i < ARP_TABLE; i++) {
        if (table[i].valid && table[i].ip == ip) {
            for (int j = 0; j < 6; j++) table[i].mac[j] = mac[j];
            return;
        }
    }
    for (int i = 0; i < ARP_TABLE; i++) {
        if (!table[i].valid) {
            table[i].valid = 1;
            table[i].ip = ip;
            for (int j = 0; j < 6; j++) table[i].mac[j] = mac[j];
            return;
        }
    }
}

int arp_request(uint32_t target_ip) {
    uint8_t frame[42];
    int p = 0;
    /* Ethernet broadcast */
    for (int i = 0; i < 6; i++) frame[p++] = 0xFF;
    for (int i = 0; i < 6; i++) frame[p++] = self_mac[i];
    frame[p++] = 0x08; frame[p++] = 0x06; /* ARP */

    frame[p++] = 0x00; frame[p++] = 0x01; /* HTYPE Ethernet */
    frame[p++] = 0x08; frame[p++] = 0x00; /* PTYPE IPv4 */
    frame[p++] = 6; frame[p++] = 4;       /* HLA PLA */
    frame[p++] = 0x00; frame[p++] = 0x01; /* OPER request */
    for (int i = 0; i < 6; i++) frame[p++] = self_mac[i];
    frame[p++] = (self_ip >> 24) & 0xFF;
    frame[p++] = (self_ip >> 16) & 0xFF;
    frame[p++] = (self_ip >> 8) & 0xFF;
    frame[p++] = self_ip & 0xFF;
    for (int i = 0; i < 6; i++) frame[p++] = 0;
    frame[p++] = (target_ip >> 24) & 0xFF;
    frame[p++] = (target_ip >> 16) & 0xFF;
    frame[p++] = (target_ip >> 8) & 0xFF;
    frame[p++] = target_ip & 0xFF;

    console_write("[ARP] Who-has request sent\n");
    return virtio_net_transmit(frame, 42);
}

void arp_input(const uint8_t* frame, uint32_t len) {
    if (!frame || len < 42) return;
    if (frame[12] != 0x08 || frame[13] != 0x06) return;
    const uint8_t* a = frame + 14;
    uint16_t oper = ((uint16_t)a[6] << 8) | a[7];
    uint32_t spa = ((uint32_t)a[14]<<24)|((uint32_t)a[15]<<16)|((uint32_t)a[16]<<8)|a[17];
    const uint8_t* sha = a + 8;
    arp_learn(spa, sha);
    if (oper == 2)
        console_write("[ARP] Reply learned\n");
}
