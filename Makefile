GCC_FALGS = -Isrc -c -ffreestanding -Wall -Wextra -O2 -nostdlib -m32 -fno-pic
OBJ_ROOT_DIR = .tmp/obj


build: check_dir
	gcc src/boot/boot.S -o target/boot.o ${GCC_FALGS}
	gcc src/boot/kernel.c -o target/kernel.o ${GCC_FALGS}
	gcc src/boot/vga/vga.c -o target/vga/vga.o ${GCC_FALGS}
	ld target/boot.o target/kernel.o target/vga/vga.o -T.config/link.ld -o .vdisk/boot/boot.bin
	grub-mkrescue -o target/MGOS.iso .vdisk/

check_dir:
	mkdir -p ${OBJ_ROOT_DIR}
	mkdir -p ${OBJ_ROOT_DIR}\vga

clean:
	rm -rf target/*

run:
	qemu-system-x86_64 -m 4096 -drive format=raw,file=target/MGOS.iso

debug:
	qemu-system-x86_64 -m 4096 -drive format=raw,file=target/MGOS.iso -S -s
