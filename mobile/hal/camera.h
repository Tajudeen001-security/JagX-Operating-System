#ifndef JAGX_HAL_CAMERA_H
#define JAGX_HAL_CAMERA_H

#include <stdint.h>

/* JagX Camera HAL — device ports implement these ops */

enum jagx_camera_id { JAGX_CAM_BACK = 0, JAGX_CAM_FRONT = 1 };

struct jagx_camera_ops {
    int (*open)(enum jagx_camera_id id);
    int (*close)(enum jagx_camera_id id);
    int (*capture_jpeg)(enum jagx_camera_id id, uint8_t* buf, uint32_t len, uint32_t* out_size);
    int (*start_preview)(enum jagx_camera_id id);
    int (*stop_preview)(enum jagx_camera_id id);
};

void jagx_camera_register(const struct jagx_camera_ops* ops);
int  jagx_camera_open(enum jagx_camera_id id);
int  jagx_camera_capture(enum jagx_camera_id id, uint8_t* buf, uint32_t len, uint32_t* out_size);

#endif
