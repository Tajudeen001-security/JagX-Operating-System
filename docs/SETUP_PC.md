# JagX — PC setup

Full guide: root [README.md](../README.md) and [TEST_WINDOWS.md](TEST_WINDOWS.md).

```bash
git clone https://github.com/Tajudeen001-security/JagX-Operating-System.git
cd JagX-Operating-System/kernel
make
make iso
qemu-system-i386 -cdrom ../boot/jagx.iso -m 256M -serial stdio \
  -netdev user,id=net0 -device virtio-net-pci,netdev=net0
```

Windows: install QEMU or VirtualBox, attach `boot/jagx.iso`. USB boot: Rufus DD mode (does not delete Windows if you only boot).
