#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

echo "[+] Detectando sistema..."

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
    LIKE=${ID_LIKE:-}
else
    echo "No se pudo detectar el sistema operativo."
    exit 1
fi

# Detectar familia real
if [[ "$OS" =~ (ubuntu|debian) || "$LIKE" =~ (debian) ]]; then
    DISTRO="debian"
elif [[ "$OS" =~ (arch) || "$LIKE" =~ (arch) ]]; then
    DISTRO="arch"
elif [[ "$OS" =~ (fedora) ]]; then
    DISTRO="fedora"
else
    echo "Distribución no soportada: $OS"
    exit 1
fi

# Verificar sudo
if ! command -v sudo >/dev/null 2>&1; then
    echo "sudo no está instalado."
    exit 1
fi

install_debian() {
    echo "[+] Instalando dependencias (Debian-based)..."
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
    echo "[+] Instalando dependencias (Arch-based)..."
    sudo pacman -Syu --noconfirm \
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

case "$DISTRO" in
    debian) install_debian ;;
    arch) install_arch ;;
    fedora) install_fedora ;;
esac

echo ""
echo "[✔] Todo listo."
echo "[→] Ejecuta ahora:"
echo "    make clean && make run"
