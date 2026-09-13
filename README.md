# JagX Operating System

**Founder:** Gbadamosi Tajudeen Olajide  
https://github.com/Tajudeen001-security/JagX-Operating-System

Original OS for PC + Mobile (not Linux/Android/iOS).

## Productivity suite (v0.0.20)

| App | Like |
|-----|------|
| **Notepad** | Windows Notepad |
| **Paint** | MS Paint |
| **JagSheet** | Excel (grids, sums, budget sample) |
| **JagBase** | Access (tables / records) |

Built for **office and government pilot demos** (Nigeria & Africa).  
See **[docs/GOVERNMENT.md](docs/GOVERNMENT.md)** — honest readiness + pilot steps.

## Quick start

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

## Also included

Lock screen · Status bar · Gallery · Screenshots · Control Center · Networking path · Security docs
