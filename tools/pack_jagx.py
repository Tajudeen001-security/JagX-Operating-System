#!/usr/bin/env python3
"""Build a JagX .jagx package. Refuses APK/IPA/EXE payloads."""
from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

MAGIC = 0x5847414A  # 'JAGX' little-endian
VERSION = 1
FORBIDDEN = {".apk", ".ipa", ".exe", ".dex", ".aab", ".msi"}


def die(msg: str, code: int = 1) -> None:
    print(f"[pack_jagx] {msg}", file=sys.stderr)
    sys.exit(code)


def main() -> None:
    p = argparse.ArgumentParser(description="Pack a native JagX .jagx app")
    p.add_argument("--manifest", required=True)
    p.add_argument("--payload", required=True)
    p.add_argument("--out", required=True)
    args = p.parse_args()

    out = Path(args.out)
    if out.suffix.lower() != ".jagx":
        die("Output must end in .jagx (not APK)")

    payload_path = Path(args.payload)
    if payload_path.suffix.lower() in FORBIDDEN:
        die(f"Refused: {payload_path.suffix} is not a JagX payload. Rebuild native.")

    manifest = Path(args.manifest).read_text(encoding="utf-8")
    if "package=" not in manifest or "name=" not in manifest:
        die("Manifest needs name= and package=")
    payload = payload_path.read_bytes()
    checksum = sum(payload) & 0xFFFFFFFF
    man = manifest.encode("utf-8")

    header = struct.pack("<IHHIII", MAGIC, VERSION, 0, len(man), len(payload), checksum)
    out.write_bytes(header + man + payload)
    print(f"[pack_jagx] wrote {out} ({out.stat().st_size} bytes)")
    print("[pack_jagx] Install on JagX with JagStore or pkg_install_path()")


if __name__ == "__main__":
    main()
