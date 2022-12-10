#include "type.h"
#include "boot/vga/vga.h"

void mango_kernel_init() {
    vga_init();
    for (u16 i=0;i<24;i++)
        put_string("Hello World !  I'm Mango OS ! \n", VGA_COLOR_LIGHT_CYAN);
}
