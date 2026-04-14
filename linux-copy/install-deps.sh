#!/usr/bin/env bash
# Dependencias para compilar MiniOS y arrancar en QEMU (Ubuntu/Debian).
set -euo pipefail
cd "$(dirname "$0")"
if [ "$(id -u)" -ne 0 ]; then
  exec sudo "$0" "$@"
fi
export DEBIAN_FRONTEND=noninteractive
apt-get update -qq
apt-get install -y \
  build-essential \
  gcc-multilib \
  g++-multilib \
  qemu-system-x86 \
  grub-pc-bin \
  xorriso \
  make
echo "OK. Siguiente: make clean && make run"
