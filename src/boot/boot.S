#include "boot/bootinfo.h"

.section .multiboot
    .align 32
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long MODE_TYPE
    .long VGA_WIDTH
    .long VGA_HEIGHT
    .long DEPTH

.section .text
    .global mango_os_start
    mango_os_start:
        mov $stack_top, %esp

        call mango_kernel_init

    mango_os_loop:
        hlt
        jmp mango_os_loop

.section .bss
    .align 16
    stack_bottom:
        .skip 16384
    stack_top:
