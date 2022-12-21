#include "vga/vga.h"
#include "boot/multiboot2.h"
#include "memory/memory.h"
#include "memory/page.h"

extern Multiboot2Context g_multiboot2_context;
MemoryContext g_memory_context;

void init_memory() {
    if (!(g_multiboot2_context.basic_memory_information.enabled && g_multiboot2_context.memory_map.enabled)) {
        println("Error Init Memory Manager.", VC_RED);
        println("Cannot Get Memory Info.", VC_RED);
        return;
    }
    init_page();
    g_memory_context.memory_bottom = g_multiboot2_context.basic_memory_information.mem_lower << 10;
    g_memory_context.memory_top = g_multiboot2_context.basic_memory_information.mem_upper << 10;
    g_memory_context.memory_size = g_memory_context.memory_top - g_memory_context.memory_bottom;
}
