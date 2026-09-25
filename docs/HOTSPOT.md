# Connect JagX to a phone hotspot (or share one)

## A) Join another phone’s hotspot

1. Phone: Settings → Mobile hotspot → ON (note SSID + password)
2. JagX Control Center → **Wi‑Fi** tile  
   - Calls `wifi_join(ssid, psk)` (default SSID configurable via `control_center_join_hotspot`)
3. Or API: `control_center_join_hotspot("YourSSID", "password")`

## B) JagX as the hotspot

Control Center → **Hotspot** tile → `wifi_start_hotspot("JagX-Share", ...)`

Other phones join that SSID when the Wi‑Fi driver supports AP mode.

## C) QEMU

No real RF — APIs log and update state. Internet in QEMU uses virtio-net user networking.

## D) Itel / Tecno / Infinix

Real hotspot join needs the **Wi‑Fi driver** on that phone port (see PORT_ITEL.md).
