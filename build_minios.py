import os
import sys
import subprocess
import platform

# Supported distributions
DISTRO_CHECK = {
    'Ubuntu': 'apt',
    'Debian': 'apt',
    'Fedora': 'dnf',
    'Arch': 'pacman'
}

# Language options
LANGUAGES = {'en': 'English', 'es': 'Español'}

# Function to check dependencies
def check_dependencies(dependencies):
    missing_dependencies = []
    for dep in dependencies:
        result = subprocess.run(['which', dep], stdout=subprocess.PIPE)
        if result.returncode != 0:
            missing_dependencies.append(dep)
    return missing_dependencies

# Function to install dependencies
def install_dependencies(missing_deps):
    if platform.system() == 'Linux':
        distro = platform.linux_distribution()[0]
        package_manager = DISTRO_CHECK.get(distro)
        if package_manager:
            print(f'Installing dependencies using {package_manager}...')
            if package_manager == 'apt':
                subprocess.run(['sudo', 'apt', 'install'] + missing_deps)
            elif package_manager == 'dnf':
                subprocess.run(['sudo', 'dnf', 'install'] + missing_deps)
            elif package_manager == 'pacman':
                subprocess.run(['sudo', 'pacman', '-S'] + missing_deps)
            else:
                print('Unsupported distribution.')
                sys.exit(1)
        else:
            print('Unsupported distribution.')
            sys.exit(1)

# Function to compile kernel
def compile_kernel():
    print('Compiling the kernel...')
    # Simulated compilation process (replace with actual commands)
    subprocess.run(['echo', 'Kernel compiled.'])

# Function to create ISO image
def create_iso():
    print('Creating ISO image...')
    # Simulated ISO creation process
    subprocess.run(['echo', 'ISO created.'])

# Function for QEMU integration
def run_qemu():
    print('Running QEMU...')
    # Simulated QEMU process
    subprocess.run(['echo', 'QEMU running.'])

# Main function
def main():
    print('Welcome to the Build System')
    print('Please select your language:')
    for key, value in LANGUAGES.items():
        print(f'{key}: {value}')
    lang = input('Language (en/es): ').strip().lower()
    if lang not in LANGUAGES:
        print('Invalid language selection. Exiting.')</code>
        sys.exit(1)

    print('\nChecking dependencies...')
    dependencies = ['gcc', 'make', 'qemu']  # Add necessary dependencies
    missing_deps = check_dependencies(dependencies)
    if missing_deps:
        print(f'Missing dependencies: {missing_deps}')
        install_dependencies(missing_deps)
    else:
        print('All dependencies are installed.')

    print('\nStarting build pipeline...')
    compile_kernel()
    create_iso()
    run_qemu()

if __name__ == '__main__':
    main()