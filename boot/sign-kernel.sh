#!/bin/bash
# Secure boot helper: hash kernel and optionally sign with openssl
# Development flow — production would use hardware-backed keys.

set -e
KERNEL="${1:-../kernel/jagx.kernel}"
OUT_DIR="${2:-.}"

if [ ! -f "$KERNEL" ]; then
  echo "Kernel not found: $KERNEL"
  exit 1
fi

mkdir -p "$OUT_DIR"

echo "[sign] SHA-256 of kernel..."
sha256sum "$KERNEL" | tee "$OUT_DIR/jagx.kernel.sha256"

if command -v openssl >/dev/null 2>&1; then
  if [ ! -f "$OUT_DIR/dev-signing.key" ]; then
    echo "[sign] Generating dev RSA key (DO NOT use for production)"
    openssl genrsa -out "$OUT_DIR/dev-signing.key" 2048
    openssl rsa -in "$OUT_DIR/dev-signing.key" -pubout -out "$OUT_DIR/dev-signing.pub"
  fi
  echo "[sign] Detached signature..."
  openssl dgst -sha256 -sign "$OUT_DIR/dev-signing.key" -out "$OUT_DIR/jagx.kernel.sig" "$KERNEL"
  openssl dgst -sha256 -verify "$OUT_DIR/dev-signing.pub" -signature "$OUT_DIR/jagx.kernel.sig" "$KERNEL" \
    && echo "[sign] Verify OK"
else
  echo "[sign] openssl not found — hash only"
fi

echo "[sign] Done. Embed hash check in bootloader for full secure boot."
