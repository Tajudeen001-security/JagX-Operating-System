/* JagX virtio-net - real PCI probe + early virtqueue structures
 * Basic TX path skeleton that can be expanded to full packet send/receive.
 */
#include "virtio_net.h"
#include "net.h"
#include "../kernel/arch/x86_64/console.h"
#include "../kernel/arch/x86_64/io.h"
#include "../kernel/mm/heap.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

#define VIRTIO_PCI_HOST_FEATURES  0
#define VIRTIO_PCI_GUEST_FEATURES 4
#define VIRTIO_PCI_QUEUE_PFN      8
#define VIRTIO_PCI_QUEUE_NUM      12
#define VIRTIO_PCI_QUEUE_SEL      14
#define VIRTIO_PCI_QUEUE_NOTIFY   16
#define VIRTIO_PCI_STATUS         18
#define VIRTIO_PCI_ISR            19

#define VIRTIO_STATUS_ACK         1
#define VIRTIO_STATUS_DRIVER      2
#define VIRTIO_STATUS_DRIVER_OK   4
#define VIRTIO_STATUS_FEATURES_OK 8

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static uint32_t pci_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) |
                                  (func << 8) | (offset & 0xFC) | 0x80000000);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

static uint16_t pci_read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t val = pci_read32(bus, slot, func, offset);
    return (val >> ((offset & 2) * 8)) & 0xFFFF;
}

/* Virtqueue descriptor */
struct virtq_desc {
    uint64_t addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next;
} __attribute__((packed));

struct virtq_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[64];
} __attribute__((packed));

struct virtq_used_elem {
    uint32_t id;
    uint32_t len;
} __attribute__((packed));

struct virtq_used {
    uint16_t flags;
    uint16_t idx;
    struct virtq_used_elem ring[64];
} __attribute__((packed));

static uint16_t virtio_iobase = 0;
static int virtio_present = 0;

int virtio_net_init(void) {
    console_write("[VIRTIO-NET] PCI scan...\n");

    for (uint8_t bus = 0; bus < 8; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vend = pci_read32(bus, slot, 0, 0);
            uint16_t vendor = vend & 0xFFFF;
            uint16_t device = (vend >> 16) & 0xFFFF;

            if (vendor != 0x1AF4) continue;
            if (device != 0x1000 && device != 0x1041) continue;

            // Read BAR0 (I/O space)
            uint32_t bar0 = pci_read32(bus, slot, 0, 0x10);
            if (bar0 & 1) {
                virtio_iobase = (uint16_t)(bar0 & ~0x3);
                virtio_present = 1;

                console_write("[VIRTIO-NET] Found at I/O base\n");

                // Reset device
                outb(virtio_iobase + VIRTIO_PCI_STATUS, 0);
                outb(virtio_iobase + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACK);
                outb(virtio_iobase + VIRTIO_PCI_STATUS,
                     VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER);

                // Feature negotiation (minimal)
                uint32_t features = inl(virtio_iobase + VIRTIO_PCI_HOST_FEATURES);
                outl(virtio_iobase + VIRTIO_PCI_GUEST_FEATURES, features & 0x1); // CSUM etc later

                outb(virtio_iobase + VIRTIO_PCI_STATUS,
                     VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER |
                     VIRTIO_STATUS_FEATURES_OK);

                // Queue 0 = RX, Queue 1 = TX (legacy layout)
                // Full ring allocation would use page-aligned memory from PMM.
                // We mark DRIVER_OK so the device is alive.
                outb(virtio_iobase + VIRTIO_PCI_STATUS,
                     VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER |
                     VIRTIO_STATUS_FEATURES_OK | VIRTIO_STATUS_DRIVER_OK);

                console_write("[VIRTIO-NET] Device initialized (rings next refinement)\n");
                return 0;
            }
        }
    }

    console_write("[VIRTIO-NET] No device found\n");
    return -1;
}

void virtio_net_poll(void) {
    if (!virtio_present) return;
    // Future: process used ring for RX packets
}

int virtio_net_transmit(const void* data, uint32_t len) {
    if (!virtio_present || !data || len == 0) return -1;
    // Future: fill TX descriptor + notify queue 1
    (void)data; (void)len;
    return 0;
}
