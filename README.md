# JagX OS v0.1.2

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

## New in 0.1.2

- **On-screen PIN keypad** — click 0–9, backspace, OK (or type **1234** + Enter)
- **Power menu UI** — Power off / Restart / Sleep / Cancel (Ctrl+Shift+Q or Control Center power tile)
- **Messages** — thread list, unread dots, select, send stub
- **Gallery** — thumbnail grid, type colors, select item

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio
```
