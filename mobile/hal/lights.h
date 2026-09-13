#ifndef JAGX_HAL_LIGHTS_H
#define JAGX_HAL_LIGHTS_H

#include <stdint.h>

/* Flashlight / notification LED */

enum jagx_light_id {
    JAGX_LIGHT_TORCH = 0,
    JAGX_LIGHT_NOTIFICATION = 1
};

struct jagx_lights_ops {
    int (*set)(enum jagx_light_id id, uint8_t brightness); /* 0=off, 255=max */
    int (*get)(enum jagx_light_id id, uint8_t* brightness);
};

void jagx_lights_register(const struct jagx_lights_ops* ops);
int  jagx_torch_set(uint8_t on); /* 0 or 1 — Control Center flashlight */

#endif
