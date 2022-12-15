ENTRY(mango_os_start)

SECTIONS {
    . = 4K;

    .text BLOCK(4K) : ALIGN(4K) {
        KEEP(*(.multiboot));
        KEEP(*(.gdt32));
        KEEP(*(.gdt64));
        *(.text);
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
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
