import subprocess
import sys
import shutil

def run_command(cmd):
    try:
        subprocess.run(cmd, check=True)
    except subprocess.CalledProcessError:
        print(f"Error ejecutando: {' '.join(cmd)}", file=sys.stderr)
        sys.exit(1)

def command_exists(cmd):
    return shutil.which(cmd) is not None

def install_dependencies():
    dependencies = ['git', 'make', 'gcc', 'python3']
    missing = [dep for dep in dependencies if not command_exists(dep)]

    if not missing:
        print("Todas las dependencias ya están instaladas.")
        return

    print(f"Faltan dependencias: {missing}")

    # Detectar si existe apt
    if command_exists("apt"):
        run_command(["sudo", "apt", "update"])
        run_command(["sudo", "apt", "install", "-y"] + missing)

    elif command_exists("pkg"):  # Termux
        run_command(["pkg", "install", "-y"] + missing)

    else:
        print("Gestor de paquetes no soportado.")
        sys.exit(1)

def compile_iso():
    print("Compilando ISO...")

    if not command_exists("make"):
        print("make no está instalado")
        sys.exit(1)

    run_command(["make", "iso"])
    print("ISO compilada correctamente.")

def main():
    print("=== MiniOS Builder ===")

    install_dependencies()
    compile_iso()

if __name__ == '__main__':
    main()
