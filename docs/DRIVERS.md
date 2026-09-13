# Driver model

```c
struct jagx_driver {
  const char* name;
  enum driver_class cls;
  int (*probe)(void);
  int (*init)(void);
  int (*shutdown)(void);
};
driver_register(&my_driver);
drivers_probe_all();
```

Classes: display, input, storage, network, audio, camera, modem, power, sensor.

QEMU builtins: virtio-net, ps2-keyboard, ps2-mouse, multiboot-fb, ramfs.

Phone ports implement modem/camera/power and register into the same table + mobile HALs.
