#include "type.h"
#include "vga/vga.h"

const u8 VGA_WIDTH = 80;
const u8 VGA_HEIGHT = 25;

void vga_init(VGAContext* vga_context) {
    vga_context->vga_buffer = (u16*)0xb8000;
    vga_context->colunm = 0;
    vga_context->row = 0;
}

inline u16 zip_data(char c, u8 color) {
    return ((u16)c) | (((u16)color) << 8);
}

void put_string(VGAContext* vga_context, char* string, u8 color) {
    u16 i = 0;
    while (string[i] != '\0') {
        put_char(vga_context, string[i], color);
        i ++;
    }
}

void put_char(VGAContext* vga_context, char c, u8 color) {
    if (c == '\n') {
        vga_context->row ++;
        vga_context->colunm = 0;
        return;
    }
    vga_context->vga_buffer[vga_context->colunm + vga_context->row * VGA_WIDTH] = zip_data(c, color);
    vga_context->colunm ++;
    if (vga_context->colunm >= VGA_WIDTH) {
        vga_context->colunm = 0;
        vga_context->row ++;
        if (vga_context->row >= VGA_HEIGHT) {
            vga_context->row = 0;
        }
    }
}
