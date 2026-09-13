#include "camera.h"
#include "../../kernel/arch/x86_64/console.h"

static const struct jagx_camera_ops* cam_ops = 0;

void jagx_camera_register(const struct jagx_camera_ops* ops) {
    cam_ops = ops;
    console_write("[HAL] Camera registered\n");
}

int jagx_camera_open(enum jagx_camera_id id) {
    if (!cam_ops || !cam_ops->open) {
        console_write("[HAL] Camera: no device driver (stub)\n");
        return -1;
    }
    return cam_ops->open(id);
}

int jagx_camera_capture(enum jagx_camera_id id, uint8_t* buf, uint32_t len, uint32_t* out_size) {
    if (!cam_ops || !cam_ops->capture_jpeg) return -1;
    return cam_ops->capture_jpeg(id, buf, len, out_size);
}
