# Test JagX on a Windows PC

You asked to override Windows for testing. Same idea as phones: start safe, only replace the disk when the ISO is a daily driver.

## Level A — Virtual machine (recommended)

### QEMU (best match for this tree)

1. Install QEMU for Windows: https://www.qemu.org/download/#windows
2. From a Developer Command Prompt in the cloned repo:

```bat
cd kernel
make
make iso
qemu-system-i386 -cdrom ..\boot\jagx.iso -m 256M -serial stdio
```

### VirtualBox

1. New VM → Other / Other/Unknown, 256 MB RAM, no disk required
2. Settings → Storage → optical → `boot/jagx.iso`
3. Start. JagX boots from CD. Windows on the host is untouched.

### Hyper-V / VMware

Attach `jagx.iso` as a DVD. Generation 1 (BIOS) is the current boot path (Multiboot). UEFI Secure Boot will refuse JagX until `docs/SECURE_BOOT.md` keys are enrolled.

## Level B — USB boot (Windows stays on disk)

1. Build `boot/jagx.iso` (`cd kernel && make iso`)
2. Use [Rufus](https://rufus.ie) → DD Image mode → write the ISO to a USB stick
   or copy the ISO onto [Ventoy](https://www.ventoy.net)
3. Reboot the PC
4. Open the firmware boot menu:
   - Dell: F12
   - HP: Esc then F9
   - Lenovo: F12
   - Surface: volume-up + power (UEFI) — may need USB-C adapter
5. Pick the USB device
6. JagX kernel starts. Do **not** format the Windows partition during a test boot.

To return to Windows: remove USB, reboot.

## Level C — Dual-boot (replace the default OS)

Only on a **lab PC**, not a ministry production workstation.

1. Back up Windows (full image)
2. Shrink the Windows partition (Disk Management)
3. Create a new partition, FAT/JagFS later; for now GRUB loads the JagX kernel from `/boot`
4. Install GRUB with `boot/grub.cfg`
5. Firmware boot order: JagX first, Windows second

`boot/grub.cfg` currently chain-loads the JagX kernel. Dual-boot with Windows Boot Manager is extra work (BCD entry or GRUB `chainloader`).

## Level D — Wipe Windows and install only JagX

**Not recommended until v1.0.** The kernel is a research OS. There is no NTFS-aware installer, no hardware certification, no office-file parity.

If a company still wants a kiosk:

1. Lab image via USB
2. Disable network until policy is signed
3. Install only signed `.jagx` from an internal JagStore
4. Keep a recovery USB with Windows or a Linux live disk

## Drivers on PC

VGA/framebuffer, i8042 keyboard/mouse, virtio-net, PIT timer are what QEMU provides. Real laptops need:

- Storage (AHCI)
- GPU (or stay in VESA framebuffer)
- Wi-Fi chip firmware
- Audio

See `docs/DRIVERS.md`.

## Companies replacing Windows

Follow [`GOVERNMENT.md`](GOVERNMENT.md): lab pilot → training → one department offline-first. Do not cut over payroll or identity systems.
