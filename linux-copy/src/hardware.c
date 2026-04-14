// hardware.c - Low level hardware layer for kernel

#include <stdint.h>

//
// ==========================
// I/O PORT FUNCTIONS
// ==========================
//

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1"
                      :
                      : "a"(val), "Nd"(port));
}

//
// ==========================
// CPU CONTROL
// ==========================
//

static inline void enable_interrupts(void) {
    __asm__ volatile ("sti");
}

static inline void disable_interrupts(void) {
    __asm__ volatile ("cli");
}

static inline void halt(void) {
    __asm__ volatile ("hlt");
}

//
// ==========================
// CONTROL REGISTERS
// ==========================
//

static inline uint32_t read_cr0(void) {
    uint32_t val;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_cr3(void) {
    uint32_t val;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(val));
    return val;
}

static inline uint32_t read_cr4(void) {
    uint32_t val;
    __asm__ volatile ("mov %%cr4, %0" : "=r"(val));
    return val;
}

//
// ==========================
// PIC (INTERRUPT CONTROLLER)
// ==========================
//

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

static void pic_remap(void) {
    // Start initialization
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // Remap offsets
    outb(PIC1_DATA, 0x20); // IRQ 0-7 -> 32-39
    outb(PIC2_DATA, 0x28); // IRQ 8-15 -> 40-47

    // Cascade config
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Mode 8086
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Clear masks
    outb(PIC1_DATA, 0x00);
    outb(PIC2_DATA, 0x00);
}

//
// ==========================
// IRQ CONTROL
// ==========================
//

static inline void irq_enable(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

static inline void irq_disable(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

//
// ==========================
// END OF INTERRUPT (EOI)
// ==========================
//

static inline void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, 0x20);
    }
    outb(PIC1_COMMAND, 0x20);
}

//
// ==========================
// CPU FEATURES
// ==========================
//

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc"
                      : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static inline void cpuid(uint32_t code,
                         uint32_t *a,
                         uint32_t *b,
                         uint32_t *c,
                         uint32_t *d) {
    __asm__ volatile ("cpuid"
                      : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                      : "a"(code));
}

//
// ==========================
// POWER CONTROL (QEMU SAFE)
// ==========================
//

static inline void reboot(void) {
    // Keyboard controller reboot
    uint8_t good = 0x02;

    while (good & 0x02)
        good = inb(0x64);

    outb(0x64, 0xFE);

    while (1) { halt(); }
}

static inline void power_off(void) {
    // QEMU / Bochs ACPI shutdown
    outw(0xB004, 0x2000);
    outw(0x604, 0x2000);

    while (1) { halt(); }
}
