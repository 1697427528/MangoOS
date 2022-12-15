.macro Discriptor BASE, LIMIT, TYPE, S, DPL, P, AVL, L, DB, G
    .word \LIMIT & 0xffff
    .word \BASE & 0xffff
    .byte (\BASE & 0xff0000) >> 16
    .byte (\TYPE & 0xf) | ((\S & 0b1) << 4) | ((\DPL & 0b11) << 5) | ((\P & 0b1) << 7)
    .byte ((\LIMIT & 0xf0000) >> 16) | ((\AVL & 0b1) << 4) | ((\L & 0b1) << 5) | ((\DB & 0b1) << 6) | ((\G & 0b1) << 7)
    .byte (\BASE & 0xff000000) >> 24
.endm


#define TI_GDT 0
#define TI_LDT 0
#define RPL_RING0 0
#define RPL_RING1 1
#define RPL_RING2 2
#define RPL_RING3 3
#define SELECTOR(offset, TI, RPL) (((offset >> 3) << 3) | ((TI & 0b1) << 2) | (RPL & 0b11))


#define ATTR_PRESENT (1)
#define ATTR_RW_READ_ONLY (0)
#define ATTR_RW_READ_WRITE (1 << 1)
#define ATTR_US_SUPERVISOR (0)
#define ATTR_US_USER (1 << 2)
#define ATTR_PWT_DISABLE (0)
#define ATTR_PWT_ENABLE (1 << 3)
#define ATTR_PCD_ENABLE_CACHE (0)
#define ATTR_PCD_DISABLE_CACHE (1 << 4)
#define ATTR_ACCESSED (1 << 5)
#define ATTR_DIRTY (1 << 6)
#define ATTR_PAGESIZE (1 << 7)
#define ATTR_PAT (1 << 7)
#define ATTR_G_LOCAL (0)
#define ATTR_G_GLOBAL (1 << 8)

#define XD_XD 1
#define XD_NONE 0

#define PDPE_NUM 128
#define PDE_NUM (PDPE_NUM << 9)


/* edi=entry_idx, ebp=table_top, attr:1 byte */
.macro PutEntry xd, sub_table_top, attr
    movl %edi, %eax
    shll $12, %eax
    addl \sub_table_top, %eax
    movl %eax, %ebx
    //shll $12, %eax
    shrl $20, %ebx
    orl $\attr, %eax
    orl $((\xd & 0b1) << 31), %ebx
    movl %eax, (%ebp, %edi, 8)
    movl %ebx, 4(%ebp, %edi, 8)
.endm


/* edi=entry_idx, ebp=pdt_top, attr:2 byte */
.macro Put2MEntry xd, attr
    movl %edi, %eax
    movl %eax, %ebx
    shll $21, %eax
    shrl $10, %ebx
    orl $\attr, %eax
    andl $0xffff, %ebx
    orl $((\xd & 0b1) << 31), %ebx
    movl %eax, (%ebp, %edi, 8)
    movl %ebx, 4(%ebp, %edi, 8)
.endm
