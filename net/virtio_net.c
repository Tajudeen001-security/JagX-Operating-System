/* Real virtio-net probe for QEMU (PCI legacy style early support)
 * This performs actual PCI config space reads to detect the device.
 * Full ring setup comes in the next networking iteration.
 */
#include "virtio_net.h"
#include "net.h"
#include "../kernel/arch/x86_64/console.h"
#include "../kernel/arch/x86_64/io.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

static uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) |
                                  (func << 8) | (offset & 0xFC) | 0x80000000);
    outl(0xCF8, address);
    return inl(0xCFC);
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

int virtio_net_init(void) {
    console_write("[VIRTIO-NET] Scanning PCI for virtio-net...\n");

    for (uint8_t bus = 0; bus < 8; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vend = pci_read(bus, slot, 0, 0);
            uint16_t vendor = vend & 0xFFFF;
            uint16_t device = (vend >> 16) & 0xFFFF;

            if (vendor == VIRTIO_VENDOR_ID &&
                (device == VIRTIO_NET_DEVICE_ID || device == 0x1041)) {
                console_write("[VIRTIO-NET] Device found on PCI\n");
                console_write("[VIRTIO-NET] Full queue setup pending next step\n");
                return 0;
            }
        }
    }

    console_write("[VIRTIO-NET] No virtio-net device present\n");
    return -1;
}

void virtio_net_poll(void) {
    // Queue processing will go here once rings are initialized
}
