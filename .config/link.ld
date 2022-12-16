ENTRY(mango_os_start_32)

SECTIONS {
    . = 4K;

    .text BLOCK(4K) : ALIGN(4K) {
        KEEP(*(.multiboot2));
        *(.text.boot);
        *(.text);
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        KEEP(*(.gdt32));
        KEEP(*(.gdt64));
        *(.rodata*);
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data);
        _edata = .;
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        KEEP(*(.pml4t));
        *(.bss);
        _end = .;
    }

}
