#include "type.h"
#include "vga/vga.h"
#include "boot/multiboot2.h"
#include "memory/memory.h"

void mango_kernel_init() {
    init_vga();
    println("Setup Into Long Mode.", VC_WHITE);
    println("Initialized VGA.", VC_WHITE);
    init_multiboot2();
    println("Loaded Multiboot2 Information.", VC_WHITE);
    init_memory();
    println("Initialized Memory Manager.", VC_WHITE);
}

void mango_kernel_main() {
    println("Welcome to MangoOS", VC_LIGHT_MAGENTA);
}
