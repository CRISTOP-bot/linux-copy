import os
import sys
import subprocess
import shutil

LANGUAGES = {'en': 'English', 'es': 'Español'}

def get_distro():
    try:
        with open("/etc/os-release") as f:
            data = f.read().lower()
            if "arch" in data:
                return "arch"
            elif "debian" in data or "ubuntu" in data or "kali" in data:
                return "debian"
            elif "fedora" in data:
                return "fedora"
    except:
        return None

def check_dependencies(dependencies):
    return [dep for dep in dependencies if shutil.which(dep) is None]

def run_command(cmd):
    result = subprocess.run(cmd)
    if result.returncode != 0:
        print(f"Error ejecutando: {' '.join(cmd)}")
        sys.exit(1)

def install_dependencies(missing):
    distro = get_distro()

    if not distro:
        print("No se pudo detectar la distro")
        sys.exit(1)

    print(f"Distro detectada: {distro}")
    print(f"Instalando: {missing}")

    if distro == "debian":
        run_command(["sudo", "apt", "update"])
        run_command(["sudo", "apt", "install", "-y"] + missing)

    elif distro == "fedora":
        run_command(["sudo", "dnf", "install", "-y"] + missing)

    elif distro == "arch":
        run_command(["sudo", "pacman", "-Sy", "--noconfirm"] + missing)

def compile_kernel():
    print("Compilando kernel...")
    run_command(["make"])

def create_iso():
    print("Creando ISO...")
    run_command(["make", "iso"])

def run_qemu():
    if shutil.which("qemu-system-x86_64") is None:
        print("QEMU no está instalado o no es compatible con tu sistema")
        return

    print("Ejecutando QEMU...")
    run_command(["qemu-system-x86_64", "-cdrom", "os.iso"])

def main():
    print("=== OS Build System ===")

    lang = input("Idioma (en/es): ").strip().lower()
    if lang not in LANGUAGES:
        print("Idioma inválido")
        sys.exit(1)

    deps = ["gcc", "make", "nasm", "qemu-system-x86_64"]
    missing = check_dependencies(deps)

    if missing:
        print("Faltan dependencias:", missing)
        install_dependencies(missing)

    compile_kernel()
    create_iso()
    run_qemu()

if __name__ == "__main__":
    main()
