#include "net.h"
#include "virtio_net.h"
#include "dns.h"
#include "tcp.h"
#include "arp.h"
#include "dhcp.h"
#include "tls.h"
#include "../kernel/arch/x86_64/console.h"

static struct net_interface interfaces[4];

void net_init(void) {
    interfaces[0].id = 0;
    interfaces[0].name[0] = 'l';
    interfaces[0].name[1] = 'o';
    interfaces[0].name[2] = 0;
    interfaces[0].ip = 0x7F000001;
    interfaces[0].netmask = 0xFF000000;
    interfaces[0].up = 1;

    console_write("[NET] Loopback up\n");
    arp_init();
    dhcp_init();
    virtio_net_init();
    tcp_init();
    tls_init();

    struct dhcp_lease lease;
    dhcp_discover(&lease);
    if (lease.valid) {
        interfaces[1].id = 1;
        interfaces[1].name[0] = 'e'; interfaces[1].name[1] = 't'; interfaces[1].name[2] = '0'; interfaces[1].name[3] = 0;
        interfaces[1].ip = lease.yiaddr;
        interfaces[1].netmask = lease.netmask;
        interfaces[1].up = 1;
        console_write("[NET] eth0 configured via DHCP/default\n");
    }
}

int net_interface_up(int id) {
    if (id < 0 || id >= 4) return 0;
    return interfaces[id].up;
}

void net_loopback_send(const void* data, uint32_t len) {
    (void)data; (void)len;
}
