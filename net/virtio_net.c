/* JagX virtio-net: real PCI init + virtqueue memory layout
 * Rings are allocated and device is brought to DRIVER_OK.
 * Packet TX/RX completion is the next refinement; structures are real.
 */
#include "virtio_net.h"
#include "net.h"
#include "../kernel/arch/x86_64/console.h"
#include "../kernel/arch/x86_64/io.h"
#include "../kernel/mm/pmm.h"
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

#define VIRTIO_STATUS_ACK         1
#define VIRTIO_STATUS_DRIVER      2
#define VIRTIO_STATUS_DRIVER_OK   4
#define VIRTIO_STATUS_FEATURES_OK 8

#define VQ_SIZE 8

struct virtq_desc {
    uint64_t addr;
    uint32_t len;
    uint16_t flags;
    uint16_t next;
} __attribute__((packed));

struct virtq_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[VQ_SIZE];
    uint16_t used_event;
} __attribute__((packed));

struct virtq_used_elem {
    uint32_t id;
    uint32_t len;
} __attribute__((packed));

struct virtq_used {
    uint16_t flags;
    uint16_t idx;
    struct virtq_used_elem ring[VQ_SIZE];
} __attribute__((packed));

struct virtqueue {
    struct virtq_desc* desc;
    struct virtq_avail* avail;
    struct virtq_used* used;
    uint16_t num;
    uint16_t free_head;
    uint16_t last_used_idx;
    void*    buffer_page;
};

static inline void outl(uint16_t p, uint32_t v) { __asm__ volatile("outl %0,%1"::"a"(v),"Nd"(p)); }
static inline uint32_t inl(uint16_t p) { uint32_t r; __asm__ volatile("inl %1,%0":"=a"(r):"Nd"(p)); return r; }
static inline void outw(uint16_t p, uint16_t v) { __asm__ volatile("outw %0,%1"::"a"(v),"Nd"(p)); }
static inline uint16_t inw(uint16_t p) { uint16_t r; __asm__ volatile("inw %1,%0":"=a"(r):"Nd"(p)); return r; }
static inline void outb(uint16_t p, uint8_t v) { __asm__ volatile("outb %0,%1"::"a"(v),"Nd"(p)); }
static inline uint8_t inb(uint16_t p) { uint8_t r; __asm__ volatile("inb %1,%0":"=a"(r):"Nd"(p)); return r; }

static uint32_t pci_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t off) {
    outl(PCI_CONFIG_ADDRESS, (uint32_t)((bus<<16)|(slot<<11)|(func<<8)|(off&0xFC)|0x80000000));
    return inl(PCI_CONFIG_DATA);
}

static uint16_t virtio_iobase = 0;
static int virtio_present = 0;
static struct virtqueue rxq, txq;

static int vq_alloc(struct virtqueue* vq) {
    /* One page for descs+avail+used (simplified layout) */
    void* page = pmm_alloc_page();
    if (!page) return -1;
    uint8_t* p = (uint8_t*)page;
    for (int i = 0; i < 4096; i++) p[i] = 0;

    vq->num = VQ_SIZE;
    vq->desc = (struct virtq_desc*)p;
    vq->avail = (struct virtq_avail*)(p + 256);
    vq->used = (struct virtq_used*)(p + 512);
    vq->buffer_page = pmm_alloc_page();
    vq->free_head = 0;
    vq->last_used_idx = 0;

    for (uint16_t i = 0; i < VQ_SIZE - 1; i++) {
        vq->desc[i].next = i + 1;
        vq->desc[i].flags = 0;
    }
    vq->desc[VQ_SIZE - 1].next = 0xFFFF;
    return 0;
}

static void vq_register(uint16_t queue_sel, struct virtqueue* vq) {
    outw(virtio_iobase + VIRTIO_PCI_QUEUE_SEL, queue_sel);
    uint16_t qnum = inw(virtio_iobase + VIRTIO_PCI_QUEUE_NUM);
    if (qnum == 0) return;
    if (qnum > VQ_SIZE) qnum = VQ_SIZE;
    /* Legacy: queue PFN = physical addr / 4096 */
    uint32_t pfn = ((uint32_t)(uintptr_t)vq->desc) / 4096;
    outl(virtio_iobase + VIRTIO_PCI_QUEUE_PFN, pfn);
}

int virtio_net_init(void) {
    console_write("[VIRTIO-NET] PCI scan + virtqueues...\n");

    for (uint8_t bus = 0; bus < 8; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vend = pci_read32(bus, slot, 0, 0);
            uint16_t vendor = vend & 0xFFFF;
            uint16_t device = (vend >> 16) & 0xFFFF;
            if (vendor != 0x1AF4) continue;
            if (device != 0x1000 && device != 0x1041) continue;

            uint32_t bar0 = pci_read32(bus, slot, 0, 0x10);
            if (!(bar0 & 1)) continue;

            virtio_iobase = (uint16_t)(bar0 & ~3u);
            virtio_present = 1;

            outb(virtio_iobase + VIRTIO_PCI_STATUS, 0);
            outb(virtio_iobase + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACK);
            outb(virtio_iobase + VIRTIO_PCI_STATUS, VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER);

            uint32_t features = inl(virtio_iobase + VIRTIO_PCI_HOST_FEATURES);
            outl(virtio_iobase + VIRTIO_PCI_GUEST_FEATURES, features & 1u);

            outb(virtio_iobase + VIRTIO_PCI_STATUS,
                 VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER | VIRTIO_STATUS_FEATURES_OK);

            if (vq_alloc(&rxq) == 0) vq_register(0, &rxq);
            if (vq_alloc(&txq) == 0) vq_register(1, &txq);

            outb(virtio_iobase + VIRTIO_PCI_STATUS,
                 VIRTIO_STATUS_ACK | VIRTIO_STATUS_DRIVER |
                 VIRTIO_STATUS_FEATURES_OK | VIRTIO_STATUS_DRIVER_OK);

            console_write("[VIRTIO-NET] Queues registered, DRIVER_OK\n");
            return 0;
        }
    }
    console_write("[VIRTIO-NET] No device\n");
    return -1;
}

void virtio_net_poll(void) {
    if (!virtio_present) return;
}

int virtio_net_transmit(const void* data, uint32_t len) {
    if (!virtio_present || !data || len == 0 || len > 1514) return -1;
    /* Copy into TX buffer page and notify — full used-ring reclaim next */
    if (!txq.buffer_page) return -1;
    uint8_t* dst = (uint8_t*)txq.buffer_page;
    const uint8_t* src = (const uint8_t*)data;
    for (uint32_t i = 0; i < len; i++) dst[i] = src[i];

    uint16_t d = txq.free_head;
    if (d == 0xFFFF) return -1;
    txq.desc[d].addr = (uint32_t)(uintptr_t)dst;
    txq.desc[d].len = len;
    txq.desc[d].flags = 0;
    txq.free_head = txq.desc[d].next;

    uint16_t idx = txq.avail->idx % VQ_SIZE;
    txq.avail->ring[idx] = d;
    txq.avail->idx++;

    outw(virtio_iobase + VIRTIO_PCI_QUEUE_NOTIFY, 1); /* TX queue */
    return 0;
}
