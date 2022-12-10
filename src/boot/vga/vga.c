#include "type.h"
#include "boot/bootinfo.h"
#include "boot/vga/vga.h"

void vga_init() {
    vga_buffer = (u16*)0xb8000;
    // vga_buffer[0] = zip_data('H', 0x0b);
    colunm = 0;
    row = 0;
}

u16 inline zip_data(unsigned char c, u8 color) {
    return ((u16)c) | (((u16)color) << 8);
}

void put_string(unsigned char* string, u8 color) {
    u16 i = 0;
    while (string[i] != '\0') {
        put_char(string[i], color);
        i ++;
    }
}

void put_char(unsigned char c, u8 color) {
    if (c == '\n') {
        row ++;
        colunm = 0;
        return;
    }
    vga_buffer[colunm + row * VGA_WIDTH] = zip_data(c, color);
    colunm ++;
    if (colunm >= VGA_WIDTH) {
        colunm = 0;
        row ++;
        if (row >= VGA_HEIGHT) {
            row = 0;
        }
    }
}
