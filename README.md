# JagX OS v0.0.26

**Founder:** Gbadamosi Tajudeen Olajide · JagX & JRILICENSE  
https://github.com/Tajudeen001-security/JagX-Operating-System

## Auth & power

- **PIN** (lab default `1234`), password, fingerprint & face frameworks
- Boot / shutdown / restart / sleep screens (`system/power`)

## Apps

Phone **Contacts + Dialer**, **Messages**, Settings (Security), Noder, Gallery, Sheet, Base, …

## Targets

Phones: Itel → Tecno / Infinix · PC: Windows-class machines

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

Lab unlock PIN: **1234**
