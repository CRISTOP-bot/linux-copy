
OUTPUT_FORMAT("elf32-i386")
ENTRY(start)

SECTIONS
{
    /* Kernel load address (1MB standard OSDev) */
    . = 0x00100000;

    /* =========================
       MULTIBOOT SECTION
       ========================= */
    .multiboot ALIGN(4) :
    {
        *(.multiboot)
    }

    /* =========================
       CODE
       ========================= */
    .text ALIGN(16) :
    {
        *(.text*)
    }

    /* =========================
       READ-ONLY DATA
       ========================= */
    .rodata ALIGN(16) :
    {
        *(.rodata*)
    }

    /* =========================
       INITIALIZED DATA
       ========================= */
    .data ALIGN(16) :
    {
        *(.data*)
    }

    /* =========================
       BSS (zero-initialized)
       ========================= */
    .bss ALIGN(16) :
    {
        *(COMMON)
        *(.bss*)
    }

    /* =========================
       END MARKER (debugging)
       ========================= */
    _end = .;
}
