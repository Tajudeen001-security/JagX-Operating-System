# JagX OS v0.0.23

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

## Noder (full desktop IDE)

Upstream: **https://github.com/JagX-JRILICENSE/Noder**  
Actions: **https://github.com/JagX-JRILICENSE/Noder/actions** (Build All Platforms / Build Windows App, v0.8.0+)

- **Windows/laptop:** use that Electron Noder now (`npm run electron:dev` / `npm run build:win`)
- **JagX OS:** `noder.jagx` + other apps as **`.jagx`** packages

## Package more apps

Installed as `.jagx`: Noder, Notepad, Paint, JagSheet, JagBase, Terminal, Calculator, Forms, Contacts, Gallery, Settings

## Wi‑Fi / phone hotspot

See **[docs/HOTSPOT.md](docs/HOTSPOT.md)**

```c
wifi_join("PhoneSSID", "password");   /* join another phone hotspot */
wifi_start_hotspot("JagX-Share", "secret"); /* share from this device */
```

## Can you use it now?

| Target | Now |
|--------|-----|
| Noder on Windows | **Yes** — JagX-JRILICENSE/Noder |
| JagX OS in QEMU | **Yes** — lab boot + packages + APIs |
| JagX on phone joining real hotspot RF | Needs Wi‑Fi driver on hardware |

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```
