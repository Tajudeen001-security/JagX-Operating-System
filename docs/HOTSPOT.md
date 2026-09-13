# Connect JagX to a phone hotspot (or share one)

## A) Join another phone’s hotspot (most common)

1. On the **phone**: Settings → Mobile hotspot / Personal hotspot → turn **ON**. Note **SSID** and **password**.
2. On **JagX**:
   - Control Center → Wi‑Fi ON, or
   - API: `wifi_join("YourPhoneSSID", "password")`
3. Scan: `wifi_scan(...)` lists nearby SSIDs (phone hotspots appear as normal Wi‑Fi names).

Under **QEMU** there is no real radio — the stack runs and logs joins. On **real hardware**, a Wi‑Fi driver must implement scan/associate.

## B) JagX as the hotspot (other phones join you)

```c
wifi_start_hotspot("JagX-Share", "secret123");
```

Stop with `wifi_stop_hotspot()`.

Needs **AP/soft-AP** support in the Wi‑Fi firmware/driver (common on phones; less common on random PC USB dongles).

## C) QEMU networking today

PC builds use **virtio-net user networking**, not Wi‑Fi RF. Hotspot APIs are ready for device ports; internet in QEMU already works via user-net.

## Security

- Prefer WPA2 passwords on phone hotspots
- Do not hard-code passwords in shared images
