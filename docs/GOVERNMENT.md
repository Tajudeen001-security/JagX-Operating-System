# JagX for government evaluation (Nigeria & Africa)

## Status — honest

JagX is an **early research operating system**. It is **not** yet ready to replace Windows/Linux desktops in ministries as a production daily driver.

Governments can **evaluate**, pilot in labs, and fund localization — starting **now** — without claiming full national rollout readiness.

## Productivity suite in tree (v0.0.20)

| App | Similar to | Purpose |
|-----|------------|--------|
| **Notepad** | Windows Notepad | Memos, circulars draft text |
| **Paint** | MS Paint | Simple diagrams / sketches |
| **JagSheet** | Excel | Tables, budgets, column sums |
| **JagBase** | Access | Simple records (staff, dept, state) |

These are **native JagX apps** (not Microsoft Office). They demonstrate office workflows on the OS.

## Suggested pilot path (Nigeria / AU members)

1. **Lab pilot** — QEMU / dedicated PCs, no critical data
2. **Training** — IT staff build/run from source (see README)
3. **Requirements** — list must-have drivers, bilingual UI (EN/Hausa/Yoruba/Igbo/French)
4. **Security review** — `docs/SECURITY.md`, secure boot path
5. **Phased hardware** — one ministry department, offline-first

## Contact / project

- Founder: **Gbadamosi Tajudeen Olajide**
- Repo: https://github.com/Tajudeen001-security/JagX-Operating-System
- License: MIT (can be adopted into public-sector forks)

## What governments should not do yet

- Deploy as sole OS on national ID / payroll production systems
- Expect Android phone replacement without a funded BSP
- Assume Microsoft-file compatibility (`.docx` / `.xlsx` import not built)

## What is available for demos today

```bash
cd kernel && make && make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio
```

Office apps initialize on boot; JagSheet includes a sample Health/Education/Roads budget grid; JagBase includes sample African staff records.
