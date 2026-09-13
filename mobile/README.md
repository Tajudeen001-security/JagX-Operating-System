# JagX Mobile

## HAL interfaces (v0.0.15)

| HAL | Purpose | Control Center |
|-----|---------|----------------|
| `hal/camera` | Open / capture / preview | — |
| `hal/lights` | Flashlight / notification LED | Torch toggle |
| `hal/radio` | SIM, signal, mobile data, SMS | Data / Airplane |
| `hal/wifi` | Wi‑Fi on/off / connect | Wi‑Fi toggle |

Device ports **register** ops with `jagx_*_register()`. Until a real driver is bound, stubs log and allow UI development.

## Control Center

`control_center_*` toggles call into HALs — same model a future Settings UI will use.

## Replacing Android on Tecno/Itel

See [`docs/REPLACE_ANDROID.md`](../docs/REPLACE_ANDROID.md).
