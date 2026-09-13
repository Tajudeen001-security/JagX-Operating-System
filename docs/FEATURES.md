# JagX useful features (v0.0.17)

## Media
| Feature | How |
|---------|-----|
| **Gallery** | Lists photos, screenshots, recordings (`mobile/gallery`) |
| **Screenshot** | Control Center tile or API `screenshot_take()` |
| **Triple-tap screenshot** | Three quick taps/clicks near the same point |
| **Screen record** | Control Center record tile; stops → saves to Gallery |
| **Camera photos** | Via camera HAL → Gallery (needs device driver for real frames) |

## System
| Feature | Notes |
|---------|-------|
| Control Center | Wi‑Fi, data, airplane, torch, screenshot, record |
| Settings | Network, Display, Sound, Security, About |
| Flashlight | Lights HAL / CC torch |
| Do Not Disturb | Policy flag |
| Battery saver | Policy flag until power HAL |
| Clipboard | System text buffer |

## Gestures
- **Triple-tap** → screenshot (enabled by default; `screenshot_set_triple_tap_enabled`)

On real phones, touch driver should call `screenshot_on_tap(x,y)` for each finger-down.
