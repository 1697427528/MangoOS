#ifndef __PAGE_H__
#define __PAGE_H__

#include "type.h"
#include "boot/pageinfo.h"

#pragma pack(push)
#pragma pack(1)

typedef struct _TableEntry {
    u8 present:1;
    u8 read_write:1;
    u8 user_super:1;
    u8 pwt:1;
    u8 pcd:1;
    u8 accessed:1;
    u8 _reserved0:6;
    u64 addr:36;
    u16 _reserved1:15;
    u8 xd:1;
} PML4TE, PDPTE;

typedef struct _PDE {
    u8 present:1;
    u8 read_write:1;
    u8 user_super:1;
    u8 pwt:1;
    u8 pcd:1;
    u8 accessed:1;
    u8 dirty:1;
    u8 ps:1;
    u8 global:1;
    u8 _reserved0:3;
    u8 pat:1;
    u16 _reserved1:8;
    u32 addr:31;
    u16 _reserved2:11;
    u8 xd:1;
} PDE;

typedef struct _VirtualAddress {
    u32 offset:21;
    u16 pde_idx:9;
    u16 pdpte_idx:9;
    u16 pml4te_idx:9;
    u16 sign:16;
} VirtualAddress;

#pragma pack(pop)

typedef PML4TE PML4T[512];
typedef PDPTE PDPT[PDPTE_NUM];
typedef PDE PD[PDE_NUM];

typedef struct _PageContext {
    PML4T *pml4t;
    PDPT *pdpt;
    PD *pd;
} PageContext;

void init_page();

u64 virtual2physical(u64 vaddr);
u64 physical2virtual(u64 paddr);

#endif
