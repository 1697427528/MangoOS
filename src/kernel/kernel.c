#include "type.h"
#include "vga/vga.h"

void mango_kernel_init() {
    VGAContext vga_context;
    vga_init(&vga_context);
    for (u16 i=0;i<24;i++) {
        put_string(&vga_context, "Hello World !  I'm Mango OS ! \n", VGA_COLOR_LIGHT_CYAN);
    }
}
