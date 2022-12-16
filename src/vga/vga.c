#include "type.h"
#include "vga/vga.h"


VGAContext g_vga_context;

void init_vga() {
    g_vga_context.vga_buffer = (u16*)0xb8000;
    g_vga_context.colunm = 0;
    g_vga_context.row = 0;
}

inline u16 zip_data(char c, VGACOLOR color) {
    return ((u16)c) | (((u16)color) << 8);
}

void print(char* string, VGACOLOR color) {
    u16 i = 0;
    while (string[i] != '\0') {
        print_char(string[i], color);
        i ++;
    }
}

void println(char* string, VGACOLOR color) {
    print(string, color);
    print_char('\n', color);
}

void print_char(char c, VGACOLOR color) {
    if (c == '\n') {
        g_vga_context.row += 1;
        g_vga_context.colunm = 0;
        return;
    }
    g_vga_context.vga_buffer[g_vga_context.colunm + g_vga_context.row * VGA_WIDTH] = zip_data(c, color);
    g_vga_context.colunm += 1;
    if (g_vga_context.colunm >= VGA_WIDTH) {
        g_vga_context.colunm = 0;
        g_vga_context.row += 1;
        if (g_vga_context.row >= VGA_HEIGHT) {
            g_vga_context.row = 0;
        }
    }
}
