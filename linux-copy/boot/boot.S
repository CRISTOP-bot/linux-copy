.section .multiboot
.align 4
.long 0x1BADB002
.long 0x00000003
.long -(0x1BADB002 + 0x00000003)

.section .text
.global start
.extern kmain

start:
    cli

    # zero segments
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    # stack setup
    mov $stack_top, %esp

    # call kernel
    call kmain

.hang:
    hlt
    jmp .hang

.section .bss
.align 16
stack:
    .skip 16384
stack_top:
