# build_minios.py

"""
This script installs dependencies and compiles the ISO with bilingual support in Spanish and English.
"""

import subprocess
import sys

def install_dependencies():
    dependencies = ['git', 'make', 'gcc', 'python3']
    for dependency in dependencies:
        try:
            subprocess.run(['apt', 'install', '-y', dependency], check=True)
            print(f'Installed {dependency}')
        except subprocess.CalledProcessError:
            print(f'Failed to install {dependency}', file=sys.stderr)


def compile_iso():
    # Assuming we have a script or Makefile to compile the ISO
    try:
        subprocess.run(['make', 'iso'], check=True)
        print('ISO compiled successfully.')
    except subprocess.CalledProcessError:
        print('Failed to compile ISO.', file=sys.stderr)


def main():
    print('Starting the installation of dependencies...')
    install_dependencies()
    print('Compiling the ISO...')
    compile_iso()


if __name__ == '__main__':
    main()