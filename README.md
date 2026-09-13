# JagX OS v0.0.21

**Founder:** Gbadamosi Tajudeen Olajide  
https://github.com/Tajudeen001-security/JagX-Operating-System

From-scratch OS for PC + Mobile. **Not** production-ready for national systems in 2 days — use for **lab pilots & demos**.

## Apps

| Users | Apps |
|-------|------|
| **Everyone** | Notepad, Paint, Calculator, Calendar, Gallery, Files, Browser path |
| **Government** | JagSheet, JagBase, Forms, Contacts |
| **Developers** | Terminal, IDE surface, driver framework |

## Bilingual UI

English · Hausa · Yoruba · Igbo · Français — `i18n/lang.c`

## Docs for your 2-day push

- [TRAINING.md](docs/TRAINING.md) — Day 1 & 2 curriculum
- [CERTIFICATION.md](docs/CERTIFICATION.md) — L0→L3 readiness
- [GOVERNMENT.md](docs/GOVERNMENT.md) — pilot policy
- [DRIVERS.md](docs/DRIVERS.md) — driver registration

## Quick start

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```
