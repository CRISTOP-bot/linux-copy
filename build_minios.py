import os
import sys
import subprocess
import platform
import shutil

LANGUAGES = {'en': 'English', 'es': 'Español'}

# Detect distro (modern way)
def get_distro():
    try:
        with open("/etc/os-release") as f:
            for line in f:
                if line.startswith("ID="):
                    return line.strip().split("=")[1].replace('"', '')
    except:
        return None

# Check dependencies
def check_dependencies(dependencies):
    missing = []
    for dep in dependencies:
        if shutil.which(dep) is None:
            missing.append(dep)
    return missing

# Install deps
def install_dependencies(missing):
    distro = get_distro()

    if not distro:
        print("Cannot detect Linux distro.")
        sys.exit(1)

    print(f"Detected distro: {distro}")

    if distro in ["ubuntu", "debian"]:
        cmd = ["sudo", "apt", "install", "-y"] + missing
    elif distro in ["fedora"]:
        cmd = ["sudo", "dnf", "install", "-y"] + missing
    elif distro in ["arch"]:
        cmd = ["sudo", "pacman", "-S", "--noconfirm"] + missing
    else:
        print("Unsupported distro")
        sys.exit(1)

    subprocess.run(cmd)

# REAL build functions
def compile_kernel():
    print("Compiling kernel...")
    result = subprocess.run(["make"], cwd=".")
    if result.returncode != 0:
        print("Kernel build failed!")
        sys.exit(1)

def create_iso():
    print("Creating ISO...")
    result = subprocess.run(["make", "iso"])
    if result.returncode != 0:
        print("ISO build failed!")
        sys.exit(1)

def run_qemu():
    print("Running QEMU...")
    subprocess.run(["qemu-system-x86_64", "-cdrom", "os.iso"])

def main():
    print("Welcome to OS Build System")

    print("Select language:")
    for k, v in LANGUAGES.items():
        print(f"{k}: {v}")

    lang = input("Language (en/es): ").strip().lower()
    if lang not in LANGUAGES:
        print("Invalid language")
        sys.exit(1)

    deps = ["gcc", "make", "qemu-system-x86_64", "nasm"]
    missing = check_dependencies(deps)

    if missing:
        print("Missing:", missing)
        install_dependencies(missing)

    print("\nBuilding OS...")
    compile_kernel()
    create_iso()
    run_qemu()

if __name__ == "__main__":
    main()
