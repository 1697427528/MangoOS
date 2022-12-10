TARGET_ROOT = .tmp/target
OBJ_ROOT = ${TARGET_ROOT}/obj
ISO_ROOT = ${TARGET_ROOT}
SRC_ROOT = src

DISK = .vdisk
BOOT_DIR = ${DISK}/boot
GRUB_DIR = ${BOOT_DIR}/grub
MANGO_OS_ELF = ${BOOT_DIR}/boot.bin
MANGO_ISO = MangoOS.iso

CC = gcc
CC_OPTIMIZE_FLAGS = -O2
CC_WARN_FLAGS = -Wall -Wextra
CC_OS_FLAGS = -nostdlib -ffreestanding -fno-pic
CC_FALGS = -I${SRC_ROOT} ${CC_OS_FLAGS} ${CC_WARN_FLAGS} -c ${CC_OPTIMIZE_FLAGS} -m32

LINK = ld
LINK_FLAGS = -T.config/link.ld

ALL_SRC_FILES = boot/boot.S boot/kernel.c boot/vga/vga.c
ALL_OBJ_FILES = $(patsubst %, ${OBJ_ROOT}/%.o, ${ALL_SRC_FILES})

build: ${MANGO_ISO}

rebuild: dist-clean build

${MANGO_ISO}: ${MANGO_OS_ELF} ${GRUB_DIR}/grub.cfg
	grub-mkrescue ${DISK} -o $@

${MANGO_OS_ELF}: ${ALL_OBJ_FILES}
	${LINK} $^ ${LINK_FLAGS} -o $@

${OBJ_ROOT}/%.o: ${SRC_ROOT}/% make-dir
	${CC} ${CC_FALGS} $< -o $@

make-dir:
	mkdir -p ${foreach filename, ${ALL_SRC_FILES}, ${OBJ_ROOT}/$(dir ${filename})}

clean: clean-obj

clean-obj:
	rm -rf ${TARGET_ROOT}
	rm -rf ${MANGO_OS_ELF}

clean-iso:
	rm -rf ${MANGO_ISO}

dist-clean: clean-obj clean-iso


QEMU_RUN_CMD = qemu-system-x86_64 -m 4096 -drive format=raw,file=${MANGO_ISO}

run:
	${QEMU_RUN_CMD}

debug:
	${QEMU_RUN_CMD} -S -s
