## Detect cross-toolchain; fall back to native gcc/g++ with -m32 if missing
ifeq ($(shell which i686-elf-gcc 2>/dev/null),)
	CC = g++
	AS = gcc
	LD = ld
	CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -m32 -nostdinc
	LDFLAGS = -m elf_i386 -nostdlib
else
	CC = i686-elf-g++
	AS = i686-elf-gcc
	LD = i686-elf-ld
	CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -m32 -nostdinc
	LDFLAGS = -m elf_i386 -nostdlib
endif

# Toolchain nativo (g++/gcc): comprobar que existen en PATH
ifeq ($(CC),g++)
ifeq ($(shell which g++ 2>/dev/null),)
$(error No se encontró g++/gcc. En Ubuntu/Debian: sudo apt install build-essential gcc-multilib g++-multilib)
endif
ifeq ($(shell which gcc 2>/dev/null),)
$(error No se encontró gcc. En Ubuntu/Debian: sudo apt install build-essential gcc-multilib g++-multilib)
endif
endif

BUILD_DIR = build
ISO_DIR = iso

KERNEL = $(BUILD_DIR)/kernel.bin

all: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(ISO_DIR)/boot/grub

$(BUILD_DIR)/boot.o: boot/boot.S | $(BUILD_DIR)
	$(AS) -m32 -ffreestanding -c boot/boot.S -o $(BUILD_DIR)/boot.o

$(BUILD_DIR)/kernel.o: src/kernel.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c src/kernel.cpp -o $(BUILD_DIR)/kernel.o

$(KERNEL): $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o linker.ld | $(BUILD_DIR)
	$(LD) $(LDFLAGS) -T linker.ld -o $(KERNEL) $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o

iso: all
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin

echo-iso: iso
	grub-mkrescue -o os.iso $(ISO_DIR)

# Sin ventana X (p. ej. por SSH): make run-curses  o  make run QEMU_OPTS=-display curses
QEMU_OPTS ?=

run: echo-iso
	qemu-system-i386 -cdrom os.iso -m 512M $(QEMU_OPTS)

run-curses: echo-iso
	qemu-system-i386 -cdrom os.iso -m 512M -display curses

clean:
	rm -rf $(BUILD_DIR) os.iso $(ISO_DIR)/boot/kernel.bin

.PHONY: all iso echo-iso run run-curses clean
