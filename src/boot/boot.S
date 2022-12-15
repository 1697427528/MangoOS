.code32
#include "boot/multiboot2.inc"
#include "boot/boot.inc"


.section .multiboot2
HEADER_LENGTH = multiboot2_header_end - multiboot2_header_start
CHECKSUM = -(HEADER_LENGTH + MAGIC + ARCHITECTURE)
multiboot2_header_start:
    .align 8
    .long MAGIC
    .long ARCHITECTURE
    .long HEADER_LENGTH
    .long CHECKSUM

    header_tag_start:
    .long 2 
    .long header_tag_end - header_tag_start
    .long multiboot2_header_start
    .long multiboot2_header_start
    .long _edata
    .long _end
    header_tag_end:

    entry_tag_start:
    .long 3
    .long entry_tag_end - entry_tag_start
    .long mango_os_start_32
    entry_tag_end:
    .long 0
multiboot2_header_end:

.section .gdt32
    GDT32:
        GDT32_NULL: Discriptor 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        GDT32_DATA: Discriptor 0, 0xfffff, 0b0011, 1, 0, 1, 0, 0, 1, 1
        GDT32_CODE: Discriptor 0, 0xfffff, 0b1011, 1, 0, 1, 0, 0, 1, 1
    GDT32_END:
    GDT32_POINTER:
        .word GDT32_END - GDT32
        .long GDT32
    GDT32_DATA_SELECTOR = SELECTOR((GDT32_DATA - GDT32_NULL), TI_GDT, RPL_RING0)
    GDT32_CODE_SELECTOR = SELECTOR((GDT32_CODE - GDT32_NULL), TI_GDT, RPL_RING0)


.section .gdt64
    GDT64:
        GDT64_NULL: Discriptor 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        GDT64_DATA: Discriptor 0, 0, 0b0000, 1, 0, 1, 0, 0, 0, 0
        GDT64_CODE: Discriptor 0, 0, 0b1000, 1, 0, 1, 0, 1, 0, 0
    GDT64_END:
    GDT64_POINTER:
        .word GDT64_END - GDT64
        .long GDT64
    GDT64_DATA_SELECTOR = SELECTOR((GDT64_DATA - GDT64_NULL), TI_GDT, RPL_RING0)
    GDT64_CODE_SELECTOR = SELECTOR((GDT64_CODE - GDT64_NULL), TI_GDT, RPL_RING0)



.section .bss
    .align 16
    stack_bottom:
        .skip 16384, 0
    stack_top:


.section .pml4t
    .align 4096
    .global pml4t_top
    pml4t_top:
        .skip 4096, 0
    .global pdpt_top
    pdpt_top:
        .skip 4096, 0
    .global pdt_top
    pdt_top:
        .skip 4096 * PDPE_NUM, 0


.section .text
    .global mango_os_start_32
    mango_os_start_32:
        cli
        lgdt GDT32_POINTER
        pushl $GDT32_CODE_SELECTOR
        pushl $_protect_code_start
        lretl
    
    _protect_code_start:
        _init_register:
            movw $GDT32_DATA_SELECTOR, %ax
            movw %ax, %es
            movw %ax, %ds
            movw %ax, %gs
            movw %ax, %fs
            movw %ax, %ss
            movl $stack_top, %esp

        _init_pml4_table:
            movl $pml4t_top, %ebp
            movl $0, %edi
            PutEntry XD_NONE, $pdpt_top, ATTR_RW_READ_WRITE | ATTR_PRESENT
            movl $pdpt_top, %ebp
            _put_pdpe:
                PutEntry XD_NONE, $pdt_top, ATTR_RW_READ_WRITE | ATTR_PRESENT
                incl %edi
                cmpl $PDPE_NUM, %edi
                jne _put_pdpe
            movl $pdt_top, %ebp
            movl $0, %edi
            _put_pde:
                Put2MEntry XD_NONE, ATTR_RW_READ_WRITE + ATTR_PRESENT + ATTR_PAGESIZE
                incl %edi
                cmpl $PDE_NUM, %edi
                jne _put_pde
        
        _goto_long_mode:
            lgdt GDT64_POINTER
            _set_PAE:
                movl %cr0, %eax
                btrl $31, %eax
                movl %eax, %cr0
                movl %cr4, %eax
                btsl $5, %eax
                movl %eax, %cr4
            _set_cr3:
                movl $pml4t_top, %eax
                movl %eax, %cr3
            _set_LME:
                movl $0xc0000080, %ecx
                rdmsr
                btsl $8, %eax
                wrmsr
            _set_entry:
                pushl $GDT64_CODE_SELECTOR
                pushl $mango_os_start_64
            _set_PG:
                movl %cr0, %eax
                btsl $31, %eax
                movl %eax, %cr0
            lretl


.code64
.section .text
    .global mango_os_start_64
    mango_os_start_64:
        mov $stack_top, %rsp
        call mango_kernel_init
    mango_os_end_loop:
        jmp mango_os_end_loop
