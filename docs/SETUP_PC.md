# JagX — PC setup (detailed)

See the root **README.md** sections 1–7 and 10 for the full guide.

Quick path:

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel
make
make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 128M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```
