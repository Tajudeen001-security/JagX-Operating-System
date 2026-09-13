#ifndef JAGX_TCP_H
#define JAGX_TCP_H

#include <stdint.h>

enum tcp_state {
    TCP_CLOSED = 0,
    TCP_SYN_SENT,
    TCP_ESTABLISHED,
    TCP_FIN_WAIT,
    TCP_CLOSE_WAIT
};

struct tcp_pcb {
    enum tcp_state state;
    uint32_t local_ip, remote_ip;
    uint16_t local_port, remote_port;
    uint32_t snd_nxt, snd_una;
    uint32_t rcv_nxt;
    uint16_t mss;
};

void tcp_init(void);
int  tcp_connect(struct tcp_pcb* pcb, uint32_t rip, uint16_t rport);
int  tcp_send(struct tcp_pcb* pcb, const void* data, uint32_t len);
int  tcp_recv(struct tcp_pcb* pcb, uint8_t* buf, uint32_t buf_len);
void tcp_input(const uint8_t* frame, uint32_t len);
void tcp_close(struct tcp_pcb* pcb);

#endif
