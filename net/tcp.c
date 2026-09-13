#include "tcp.h"
#include "../kernel/arch/x86_64/console.h"

void tcp_init(void) {
    console_write("[TCP] Protocol control block layer ready\n");
}

int tcp_connect(struct tcp_pcb* pcb, uint32_t rip, uint16_t rport) {
    if (!pcb) return -1;
    pcb->state = TCP_SYN_SENT;
    pcb->remote_ip = rip;
    pcb->remote_port = rport;
    pcb->local_port = 40000;
    pcb->snd_nxt = 1;
    pcb->rcv_nxt = 0;
    console_write("[TCP] SYN-SENT (segment TX needs IPv4 + virtio TX path complete)\n");
    return 0;
}

int tcp_send(struct tcp_pcb* pcb, const void* data, uint32_t len) {
    if (!pcb || pcb->state != TCP_ESTABLISHED) return -1;
    (void)data; (void)len;
    return -1;
}
