#ifndef JAGX_TCP_H
#define JAGX_TCP_H

#include <stdint.h>

enum tcp_state {
    TCP_CLOSED = 0,
    TCP_SYN_SENT,
    TCP_ESTABLISHED,
    TCP_FIN_WAIT
};

struct tcp_pcb {
    enum tcp_state state;
    uint32_t local_ip, remote_ip;
    uint16_t local_port, remote_port;
    uint32_t snd_nxt, rcv_nxt;
};

void tcp_init(void);
int  tcp_connect(struct tcp_pcb* pcb, uint32_t rip, uint16_t rport);
int  tcp_send(struct tcp_pcb* pcb, const void* data, uint32_t len);

#endif
