/* Built-in stub drivers so the framework is non-empty on QEMU */
#include "driver.h"

static int stub_ok(void) { return 0; }

static struct jagx_driver virtio_net_drv = {
    .name = "virtio-net", .cls = DRV_NETWORK,
    .probe = stub_ok, .init = stub_ok, .shutdown = 0, .priv = 0
};
static struct jagx_driver ps2_kbd = {
    .name = "ps2-keyboard", .cls = DRV_INPUT,
    .probe = stub_ok, .init = stub_ok, .shutdown = 0, .priv = 0
};
static struct jagx_driver ps2_mouse = {
    .name = "ps2-mouse", .cls = DRV_INPUT,
    .probe = stub_ok, .init = stub_ok, .shutdown = 0, .priv = 0
};
static struct jagx_driver fb_drv = {
    .name = "multiboot-fb", .cls = DRV_DISPLAY,
    .probe = stub_ok, .init = stub_ok, .shutdown = 0, .priv = 0
};
static struct jagx_driver ramdisk = {
    .name = "ramfs", .cls = DRV_STORAGE,
    .probe = stub_ok, .init = stub_ok, .shutdown = 0, .priv = 0
};

void drivers_register_builtins(void) {
    driver_register(&virtio_net_drv);
    driver_register(&ps2_kbd);
    driver_register(&ps2_mouse);
    driver_register(&fb_drv);
    driver_register(&ramdisk);
}
