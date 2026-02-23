.section .multiboot
.align 4
.long 0x1BADB002
.long 0x00010003
.long -(0x1BADB002 + 0x00010003)

.text
.global start
.type start, @function
start:
    cli
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov $stack_top, %esp
    call kmain
1:  jmp 1b

.section .bss
.lcomm stack, 8192
.set stack_top, stack + 8192
