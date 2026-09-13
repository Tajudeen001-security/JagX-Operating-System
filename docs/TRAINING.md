# JagX Training Guide (2-day intensive + ongoing)

## Day 1 — Build & boot (IT staff)

1. Install Linux tools (see README)
2. `git clone` + `cd kernel && make && make iso`
3. Run under QEMU with virtio-net
4. Unlock lock screen, open Control Center, take screenshot
5. Open Notepad, Paint, JagSheet, JagBase

## Day 2 — Admin & pilot

1. Read `docs/GOVERNMENT.md` and `docs/SECURITY.md`
2. Switch UI language: EN / Hausa / Yoruba / Igbo / French (`lang_set`)
3. Review driver list (`drivers_list_console`)
4. Map department needs → apps (Forms, Sheet, Contacts, Terminal)
5. Write pilot checklist (lab only — no production payroll yet)

## Roles

| Role | Focus |
|------|--------|
| System admin | Build, ISO, QEMU, backups of RamFS exports |
| Developer | Terminal, IDE surface, driver registration |
| End user | Notepad, Paint, Sheet, Gallery, Calculator |
| Government clerk | Forms, Contacts, JagBase records |

## Certification path (organizational)

See `docs/CERTIFICATION.md`.
