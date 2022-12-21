#ifndef __PAGEINFO_H__
#define __PAGEINFO_H__

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
#define ATTR_PAT (1 << 12)
#define ATTR_G_LOCAL (0)
#define ATTR_G_GLOBAL (1 << 8)

#define XD_XD 1
#define XD_NONE 0

#define PDPTE_NUM 128
#define PDE_NUM (PDPTE_NUM << 9)

#endif
