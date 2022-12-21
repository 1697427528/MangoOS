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


#include "boot/pageinfo.h"


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
    shrl $11, %ebx
    orl $\attr, %eax
    andl $0x1fffff, %ebx
    orl $((\xd & 0b1) << 31), %ebx
    movl %eax, (%ebp, %edi, 8)
    movl %ebx, 4(%ebp, %edi, 8)
.endm
