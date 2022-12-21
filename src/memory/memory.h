#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "type.h"

typedef struct _MemoryContext {
    u64 memory_bottom;
    u64 memory_top;
    u64 memory_size;
} MemoryContext;

void init_memory();

#endif
