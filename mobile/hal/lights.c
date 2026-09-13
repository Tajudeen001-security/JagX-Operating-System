#include "lights.h"
#include "../../kernel/arch/x86_64/console.h"

static const struct jagx_lights_ops* lights_ops = 0;
static uint8_t torch_state = 0;

void jagx_lights_register(const struct jagx_lights_ops* ops) {
    lights_ops = ops;
    console_write("[HAL] Lights registered\n");
}

int jagx_torch_set(uint8_t on) {
    torch_state = on ? 255 : 0;
    if (!lights_ops || !lights_ops->set) {
        console_write(on ? "[HAL] Torch ON (stub)\n" : "[HAL] Torch OFF (stub)\n");
        return 0; /* soft success in stub so UI can develop */
    }
    return lights_ops->set(JAGX_LIGHT_TORCH, torch_state);
}
