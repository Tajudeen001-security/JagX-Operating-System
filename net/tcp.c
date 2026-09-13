/* JagX TCP — SYN / SYN-ACK / ACK / data path (minimal but real segments) */
#include "tcp.h"
#include "ipv4.h"
#include "virtio_net.h"
#include "../kernel/arch/x86_64/console.h"

#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10

static struct tcp_pcb* active_pcb = 0;
static uint32_t local_ip_default = (10u << 24) | (0u << 16) | (2u << 8) | 15u;
static uint16_t ephemeral = 40000;

static uint16_t htons16(uint16_t x) { return (uint16_t)((x << 8) | (x >> 8)); }
static uint32_t htonl32(uint32_t x) {
    return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) |
           ((x & 0xFF0000) >> 8) | ((x >> 24) & 0xFF);
}

static uint16_t tcp_checksum(uint32_t src, uint32_t dst, const uint8_t* tcp, uint16_t tcp_len) {
    uint32_t sum = 0;
    sum += (src >> 16) & 0xFFFF;
    sum += src & 0xFFFF;
    sum += (dst >> 16) & 0xFFFF;
    sum += dst & 0xFFFF;
    sum += 6; /* protocol TCP */
    sum += tcp_len;
    const uint16_t* p = (const uint16_t*)tcp;
    uint16_t left = tcp_len;
    while (left > 1) { sum += *p++; left -= 2; }
    if (left) sum += *(const uint8_t*)p;
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum);
}

static int tcp_emit(struct tcp_pcb* pcb, uint8_t flags,
                    const uint8_t* payload, uint16_t plen) {
    uint8_t seg[1500];
    uint16_t off = 5; /* 20-byte header */
    uint16_t tcp_len = (uint16_t)(off * 4 + plen);

    seg[0] = (pcb->local_port >> 8) & 0xFF;
    seg[1] = pcb->local_port & 0xFF;
    seg[2] = (pcb->remote_port >> 8) & 0xFF;
    seg[3] = pcb->remote_port & 0xFF;
    uint32_t seq = htonl32(pcb->snd_nxt);
    seg[4] = (seq >> 24) & 0xFF; seg[5] = (seq >> 16) & 0xFF;
    seg[6] = (seq >> 8) & 0xFF;  seg[7] = seq & 0xFF;
    uint32_t ack = htonl32(pcb->rcv_nxt);
    seg[8] = (ack >> 24) & 0xFF; seg[9] = (ack >> 16) & 0xFF;
    seg[10] = (ack >> 8) & 0xFF; seg[11] = ack & 0xFF;
    seg[12] = (uint8_t)(off << 4);
    seg[13] = flags;
    seg[14] = 0xFF; seg[15] = 0xFF; /* window */
    seg[16] = 0; seg[17] = 0;       /* checksum */
    seg[18] = 0; seg[19] = 0;       /* urgent */

    for (uint16_t i = 0; i < plen; i++)
        seg[20 + i] = payload[i];

    uint16_t csum = tcp_checksum(pcb->local_ip, pcb->remote_ip, seg, tcp_len);
    seg[16] = (csum >> 8) & 0xFF;
    seg[17] = csum & 0xFF;

    int rc = ipv4_send(pcb->remote_ip, 6, seg, tcp_len, pcb->local_ip);
    if (flags & TCP_SYN) pcb->snd_nxt++;
    if (plen) pcb->snd_nxt += plen;
    if (flags & TCP_FIN) pcb->snd_nxt++;
    return rc;
}

void tcp_init(void) {
    active_pcb = 0;
    console_write("[TCP] Stack ready (SYN/ACK/data)\n");
}

int tcp_connect(struct tcp_pcb* pcb, uint32_t rip, uint16_t rport) {
    if (!pcb) return -1;
    pcb->state = TCP_SYN_SENT;
    pcb->local_ip = local_ip_default;
    pcb->remote_ip = rip;
    pcb->local_port = ephemeral++;
    pcb->remote_port = rport;
    pcb->snd_nxt = 1000;
    pcb->snd_una = pcb->snd_nxt;
    pcb->rcv_nxt = 0;
    pcb->mss = 1460;
    active_pcb = pcb;

    console_write("[TCP] SYN → remote\n");
    return tcp_emit(pcb, TCP_SYN, 0, 0);
}

void tcp_input(const uint8_t* frame, uint32_t len) {
    if (!active_pcb || !frame || len < 14 + 20 + 20) return;
    const uint8_t* ip = frame + 14;
    if ((ip[0] & 0xF0) != 0x40 || ip[9] != 6) return;
    uint8_t ihl = (ip[0] & 0xF) * 4;
    const uint8_t* tcp = ip + ihl;
    uint16_t sport = ((uint16_t)tcp[0] << 8) | tcp[1];
    uint16_t dport = ((uint16_t)tcp[2] << 8) | tcp[3];
    if (dport != active_pcb->local_port || sport != active_pcb->remote_port) return;

    uint32_t seq = ((uint32_t)tcp[4]<<24)|((uint32_t)tcp[5]<<16)|((uint32_t)tcp[6]<<8)|tcp[7];
    uint32_t ack = ((uint32_t)tcp[8]<<24)|((uint32_t)tcp[9]<<16)|((uint32_t)tcp[10]<<8)|tcp[11];
    uint8_t data_off = (tcp[12] >> 4) * 4;
    uint8_t flags = tcp[13];
    uint16_t total_ip = ((uint16_t)ip[2] << 8) | ip[3];
    int payload_len = (int)total_ip - ihl - data_off;
    if (payload_len < 0) payload_len = 0;

    struct tcp_pcb* pcb = active_pcb;

    if (pcb->state == TCP_SYN_SENT && (flags & TCP_SYN) && (flags & TCP_ACK)) {
        pcb->rcv_nxt = seq + 1;
        pcb->snd_una = ack;
        pcb->state = TCP_ESTABLISHED;
        console_write("[TCP] ESTABLISHED (got SYN-ACK)\n");
        tcp_emit(pcb, TCP_ACK, 0, 0);
        return;
    }

    if (pcb->state == TCP_ESTABLISHED) {
        if (payload_len > 0) {
            pcb->rcv_nxt = seq + (uint32_t)payload_len;
            tcp_emit(pcb, TCP_ACK, 0, 0);
            console_write("[TCP] Data received, ACK sent\n");
        }
        if (flags & TCP_FIN) {
            pcb->rcv_nxt++;
            pcb->state = TCP_CLOSE_WAIT;
            tcp_emit(pcb, TCP_ACK, 0, 0);
        }
    }
}

int tcp_send(struct tcp_pcb* pcb, const void* data, uint32_t len) {
    if (!pcb || pcb->state != TCP_ESTABLISHED) return -1;
    if (!data || len == 0) return -1;
    if (len > 1400) len = 1400;
    console_write("[TCP] Sending data\n");
    return tcp_emit(pcb, TCP_ACK | TCP_PSH, (const uint8_t*)data, (uint16_t)len);
}

int tcp_recv(struct tcp_pcb* pcb, uint8_t* buf, uint32_t buf_len) {
    if (!pcb || !buf || buf_len == 0) return 0;
    uint8_t frame[1600];
    for (int i = 0; i < 30; i++) {
        uint32_t n = virtio_net_receive(frame, sizeof(frame));
        if (n > 0) {
            tcp_input(frame, n);
            /* If frame had payload, copy from TCP payload offset */
            if (n > 14 + 20 + 20) {
                const uint8_t* ip = frame + 14;
                uint8_t ihl = (ip[0] & 0xF) * 4;
                const uint8_t* tcp = ip + ihl;
                uint8_t data_off = (tcp[12] >> 4) * 4;
                const uint8_t* payload = tcp + data_off;
                uint16_t total_ip = ((uint16_t)ip[2] << 8) | ip[3];
                int plen = (int)total_ip - ihl - data_off;
                if (plen > 0) {
                    uint32_t c = (uint32_t)plen < buf_len ? (uint32_t)plen : buf_len;
                    for (uint32_t j = 0; j < c; j++) buf[j] = payload[j];
                    return (int)c;
                }
            }
        }
        for (volatile int d = 0; d < 80000; d++);
    }
    return 0;
}

void tcp_close(struct tcp_pcb* pcb) {
    if (!pcb) return;
    if (pcb->state == TCP_ESTABLISHED || pcb->state == TCP_CLOSE_WAIT)
        tcp_emit(pcb, TCP_FIN | TCP_ACK, 0, 0);
    pcb->state = TCP_CLOSED;
    if (active_pcb == pcb) active_pcb = 0;
}
