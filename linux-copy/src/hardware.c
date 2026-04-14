// hardware.c

#include <stdint.h>

// I/O operations
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" :: "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" :: "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" :: "a"(val), "Nd"(port));
}

// CPU control
static inline void enable_interrupts() {
    __asm__ volatile ("sti");
}

static inline void disable_interrupts() {
    __asm__ volatile ("cli");
}

static inline void halt() {
    __asm__ volatile ("hlt");
}

// Register operations
static inline uint32_t read_cr0() {
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    return cr0;
}

static inline uint32_t read_cr3() {
    uint32_t cr3;
    __asm__ volatile ("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

static inline uint32_t read_cr4() {
    uint32_t cr4;
    __asm__ volatile ("mov %%cr4, %0" : "=r"(cr4));
    return cr4;
}

// Paging and memory management functions
// ... additional implementations for paging and memory management handling

// GDT/IDT operations
// ... additional implementations for GDT/IDT handling

// PIC initialization and configuration
static void pic_init() {
    // Initialize the Programmable Interrupt Controller
    outb(0x20, 0x11); // Start initialization
    outb(0x21, 0x20); // Set vector offset
    outb(0x21, 0x04); // Setup cascade
    outb(0x21, 0x01); // Finish initialization
}

// IRQ enable/disable
static inline void irq_enable(int irq) {
    // Enable the specified IRQ
    outb(0x21, inb(0x21) & ~(1 << irq));
}

static inline void irq_disable(int irq) {
    // Disable the specified IRQ
    outb(0x21, inb(0x21) | (1 << irq));
}

// EOI signals
static inline void eoi(uint8_t irq) {
    outb(0x20, 0x20); // Send EOI signal to PIC
}

// Performance counters
static inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static inline uint64_t rdpmc(uint32_t counter) {
    uint32_t lo, hi;
    __asm__ volatile ("rdpmc" : "=a"(lo), "=d"(hi) : "c"(counter));
    return ((uint64_t)hi << 32) | lo;
}

// CPUID support
static inline void cpuid(uint32_t info, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx) {
    __asm__ volatile ("cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "a"(info));
}

// System control functions
static inline void reboot() {
    // Reboot the system
    __asm__ volatile ("reboot");
}

static inline void power_off() {
    // Power off the system
    __asm__ volatile ("poweroff");
}

// Additional functions and definitions as required
