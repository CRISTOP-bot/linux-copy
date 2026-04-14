#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

echo "[+] Detectando sistema..."

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    echo "No se pudo detectar el sistema operativo."
    exit 1
fi

install_debian() {
    echo "[+] Instalando dependencias (Debian/Ubuntu)..."
    sudo apt-get update -qq
    sudo apt-get install -y \
        build-essential \
        gcc-multilib \
        g++-multilib \
        qemu-system-x86_64 \
        grub-pc-bin \
        xorriso \
        make
}

install_arch() {
    echo "[+] Instalando dependencias (Arch)..."
    sudo pacman -Sy --noconfirm \
        base-devel \
        qemu \
        grub \
        xorriso \
        make
}

install_fedora() {
    echo "[+] Instalando dependencias (Fedora)..."
    sudo dnf install -y \
        gcc \
        gcc-c++ \
        make \
        qemu-system-x86_64 \
        grub2-tools \
        xorriso
}

case "$OS" in
    ubuntu|debian)
        install_debian
        ;;
    arch)
        install_arch
        ;;
    fedora)
        install_fedora
        ;;
    *)
        echo "Distribución no soportada: $OS"
        exit 1
        ;;
esac

echo ""
echo "[✔] Todo listo."
echo "[→] Ejecuta ahora:"
echo "    make clean && make run"
