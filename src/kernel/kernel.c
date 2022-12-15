#include "type.h"
#include "vga/vga.h"
#include "memory/memory.h"

void mango_kernel_init() {
    init_vga();
    init_memory();
}

void mango_kernel_main() {
    for (u16 i=0;i<24;i++) {
        put_string("Hello World !  I'm Mango OS ! \n", VC_LIGHT_CYAN);
    }
}
