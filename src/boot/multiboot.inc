#ifndef __BOOTINFO_H__
#define __BOOTINFO_H__

#define MAGIC 0x1BADB002
#define FLAGS 0x00010007
#define CHECKSUM (-(MAGIC + FLAGS))

#define MODE_TYPE 1
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define DEPTH 0


#endif
