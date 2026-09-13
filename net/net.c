#include "net.h"
#include "../kernel/arch/x86_64/console.h"

static struct net_interface interfaces[4];

void net_init(void) {
    // Loopback
    interfaces[0].id = 0;
    interfaces[0].name[0] = 'l'; interfaces[0].name[1] = 'o';
    interfaces[0].name[2] = 0;
    interfaces[0].ip = 0x7F000001;      // 127.0.0.1
    interfaces[0].netmask = 0xFF000000;
    interfaces[0].up = 1;

    console_write("[NET] Networking stub initialized (loopback)\n");
}

int net_interface_up(int id) {
    if (id < 0 || id >= 4) return 0;
    return interfaces[id].up;
}

void net_loopback_send(const void* data, uint32_t len) {
    (void)data;
    (void)len;
    // Future: deliver to local sockets
}
