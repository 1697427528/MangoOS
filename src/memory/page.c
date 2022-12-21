#include "memory/page.h"

extern PML4T pml4t_top;
extern PDPT pdpt_top;
extern PD pd_top;
PageContext g_page_context;

void init_page() {
    g_page_context.pml4t = &pml4t_top;
    g_page_context.pdpt = &pdpt_top;
    g_page_context.pd = &pd_top;
}

u64 virtual2physical(u64 vaddr) { 
    VirtualAddress *addr = (VirtualAddress*)&vaddr;
    return ((u64)addr->offset) | ((u64)((*g_page_context.pd)[addr->pdpte_idx * 512 + addr->pde_idx].addr << 21));
}

u64 physical2virtual(u64 paddr) {
    return 0;
}
