#ifndef __VGA_H__
#define __VGA_H__

#include "type.h"


#define VGA_WIDTH 80
#define VGA_HEIGHT 25


typedef enum _VGACOLOR {
	VC_BLACK = 0,
	VC_BLUE = 1,
	VC_GREEN = 2,
	VC_CYAN = 3,
	VC_RED = 4,
	VC_MAGENTA = 5,
	VC_BROWN = 6,
	VC_LIGHT_GREY = 7,
	VC_DARK_GREY = 8,
	VC_LIGHT_BLUE = 9,
	VC_LIGHT_GREEN = 10,
	VC_LIGHT_CYAN = 11,
	VC_LIGHT_RED = 12,
	VC_LIGHT_MAGENTA = 13,
	VC_LIGHT_BROWN = 14,
	VC_WHITE = 15,
} VGACOLOR;


typedef struct _VGAContext {
	u16* vga_buffer;
	u8 colunm;
	u8 row;
} VGAContext;


void init_vga();

u16 zip_data(char c, VGACOLOR color);

void put_string(char* string, VGACOLOR color);

void put_char(char c, VGACOLOR color);

#endif
