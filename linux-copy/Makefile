
# =========================
# TOOLCHAIN DETECTION
# =========================

CROSS ?= i686-elf

ifeq ($(shell which $(CROSS)-gcc 2>/dev/null),)
	CC = gcc
	CXX = g++
	AS = gcc
	LD = ld
	CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -m32 -nostdinc
	LDFLAGS = -m elf_i386 -nostdlib
else
	CC = $(CROSS)-gcc
	CXX = $(CROSS)-g++
	AS = $(CROSS)-gcc
	LD = $(CROSS)-ld
	CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -nostdlib
	LDFLAGS = -nostdlib
endif

# =========================
# DIRS
# =========================

BUILD_DIR = build
ISO_DIR = iso
KERNEL = $(BUILD_DIR)/kernel.bin

# =========================
# BUILD RULES
# =========================

all: $(KERNEL)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(ISO_DIR)/boot/grub

# bootloader
$(BUILD_DIR)/boot.o: boot/boot.S | $(BUILD_DIR)
	$(AS) -m32 -c $< -o $@

# kernel
$(BUILD_DIR)/kernel.o: src/kernel.cpp | $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

# link kernel
$(KERNEL): $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o linker.ld
	$(LD) -T linker.ld -o $@ $^

# =========================
# ISO
# =========================

iso: all
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg

	grub-mkrescue -o os.iso $(ISO_DIR)

# =========================
# RUN
# =========================

run: iso
	qemu-system-i386 -cdrom os.iso -m 512M

run-curses: iso
	qemu-system-i386 -cdrom os.iso -m 512M -display curses

# =========================
# CLEAN
# =========================

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(ISO_DIR)/boot/kernel.bin
	rm -rf os.iso

.PHONY: all iso run run-curses clean
