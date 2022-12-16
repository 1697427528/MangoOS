#include "type.h"
#include "boot/multiboot2.h"
#include "vga/vga.h"


extern u8* multiboot2_info_pointer;
u16 multiboot2_info_pointer_index;
Multiboot2Context g_multiboot2Context;


static u32 get_bytes(u8 byte_num) {
    u32 result = 0;
    multiboot2_info_pointer_index += byte_num;
    for (int i = 1; i <= byte_num; i ++) {
        result <<= 8;
        result |= multiboot2_info_pointer[multiboot2_info_pointer_index - i];
    }
    return result;
}

static void align(u8 align_byte_num) {
    if (multiboot2_info_pointer_index % align_byte_num != 0)
        multiboot2_info_pointer_index += align_byte_num - (multiboot2_info_pointer_index % align_byte_num);
}

static void unknown_type(u32 type) {
    char str[8];
    u8 i = 7;
    str[i] = '\0';
    do {
        i -= 1;
        str[i] = 48 + (type % 10);
        type /= 10;
    } while (type > 0);
    print("Unknown Multiboot2 Info Type: ", VC_LIGHT_BROWN);
    println(&str[i], VC_LIGHT_BROWN);
}

static void setup_boot_command_line(u32 size) {
    g_multiboot2Context.boot_cammand_line.enabled = TRUE;
    g_multiboot2Context.boot_cammand_line.length = size;
    g_multiboot2Context.boot_cammand_line.string = (char*)(&multiboot2_info_pointer[multiboot2_info_pointer_index]);
    get_bytes(size);
}

static void setup_boot_loader_name(u32 size) {
    g_multiboot2Context.boot_loader_name.enabled = TRUE;
    g_multiboot2Context.boot_loader_name.length = size;
    g_multiboot2Context.boot_loader_name.string = (char*)(&multiboot2_info_pointer[multiboot2_info_pointer_index]);
    get_bytes(size);
}

static void setup_basic_memory_information() {
    g_multiboot2Context.basic_memory_information.enabled = TRUE;
    g_multiboot2Context.basic_memory_information.mem_lower = get_bytes(4);
    g_multiboot2Context.basic_memory_information.mem_upper = get_bytes(4);
}

static void setup_bios_boot_device() {
    g_multiboot2Context.bios_boot_device.enabled = TRUE;
    g_multiboot2Context.bios_boot_device.biosdev = get_bytes(4);
    g_multiboot2Context.bios_boot_device.partition = get_bytes(4);
    g_multiboot2Context.bios_boot_device.sub_partition = get_bytes(4);
}

static void setup_memory_map(u32 size) {
    g_multiboot2Context.memory_map.enabled = TRUE;
    g_multiboot2Context.memory_map.entry_size = get_bytes(4);
    g_multiboot2Context.memory_map.entry_version = get_bytes(4);
    if (g_multiboot2Context.memory_map.entry_version == 0 && g_multiboot2Context.memory_map.entry_size == 24)
    {
        g_multiboot2Context.memory_map.length = (size - 8) / 24;
        g_multiboot2Context.memory_map.entries = (MemoryEntry*)(&multiboot2_info_pointer[multiboot2_info_pointer_index]);
    }
    else {
        g_multiboot2Context.memory_map.enabled = FALSE;
    }
    get_bytes(size - 8);
}

static void setup_frame_buffer_info(u32 size) {
    g_multiboot2Context.frame_buffer_info.framebuffer_addr = (((u64)get_bytes(4)) << 32) | get_bytes(4);
    g_multiboot2Context.frame_buffer_info.framebuffer_pitch = get_bytes(4);
    g_multiboot2Context.frame_buffer_info.framebuffer_width = get_bytes(4);
    g_multiboot2Context.frame_buffer_info.framebuffer_height = get_bytes(4);
    g_multiboot2Context.frame_buffer_info.framebuffer_bpp = get_bytes(1);
    g_multiboot2Context.frame_buffer_info.framebuffer_type = get_bytes(1);
    g_multiboot2Context.frame_buffer_info._reserved = get_bytes(2);
    switch (g_multiboot2Context.frame_buffer_info.framebuffer_type) {
        case FBT_INDEXED:
            g_multiboot2Context.frame_buffer_info.color_info.idx_color_info.framebuffer_palette_num_colors = get_bytes(2);
            g_multiboot2Context.frame_buffer_info.color_info.idx_color_info.framebuffer_palettes = (FrameBufferPalette*)(&multiboot2_info_pointer[multiboot2_info_pointer_index]);
            get_bytes(size - 26);
            break;
        case FBT_RGB:
            g_multiboot2Context.frame_buffer_info.color_info.rgb_color_info.framebuffer_red_field_position = get_bytes(1);
            g_multiboot2Context.frame_buffer_info.color_info.rgb_color_info.framebuffer_red_mask_size = get_bytes(1);
            g_multiboot2Context.frame_buffer_info.color_info.rgb_color_info.framebuffer_green_field_position = get_bytes(1);
            g_multiboot2Context.frame_buffer_info.color_info.rgb_color_info.framebuffer_green_mask_size = get_bytes(1);
            g_multiboot2Context.frame_buffer_info.color_info.rgb_color_info.framebuffer_blue_field_position = get_bytes(1);
            g_multiboot2Context.frame_buffer_info.color_info.rgb_color_info.framebuffer_blue_mask_size = get_bytes(1);
            get_bytes(size - 30);
            break;
        case FBT_EGA_TEXT:
            get_bytes(size - 24);
            break;
    }
}

static void setup_elf_symbols(u32 size) {
    g_multiboot2Context.elf_symbols.enabled = TRUE;
    g_multiboot2Context.elf_symbols.num = get_bytes(2);
    g_multiboot2Context.elf_symbols.entsize = get_bytes(2);
    g_multiboot2Context.elf_symbols.shndx = get_bytes(2);
    g_multiboot2Context.elf_symbols._reserved = get_bytes(2);
    g_multiboot2Context.elf_symbols.length = size - 8;
    g_multiboot2Context.elf_symbols.section_headers = (char*)(&multiboot2_info_pointer[multiboot2_info_pointer_index]);
    get_bytes(size - 8);
}

static void setup_apm_name() {
    g_multiboot2Context.apm_table.enabled = TRUE;
    g_multiboot2Context.apm_table.version = get_bytes(2);
    g_multiboot2Context.apm_table.cseg = get_bytes(2);
    g_multiboot2Context.apm_table.offset = get_bytes(4);
    g_multiboot2Context.apm_table.cseg_16 = get_bytes(2);
    g_multiboot2Context.apm_table.dseg = get_bytes(2);
    g_multiboot2Context.apm_table.flags = get_bytes(2);
    g_multiboot2Context.apm_table.cseg_len = get_bytes(2);
    g_multiboot2Context.apm_table.cseg_16_len = get_bytes(2);
    g_multiboot2Context.apm_table.dseg_len = get_bytes(2);
}

static void setup_acpi_old_rsdp(u32 size) {
    g_multiboot2Context.acpi_old_rsdp.enabled = TRUE;
    g_multiboot2Context.acpi_old_rsdp.length = size;
    g_multiboot2Context.acpi_old_rsdp.copy_of_rsdpv1 = (u8*)(&multiboot2_info_pointer[multiboot2_info_pointer_index]);
    get_bytes(size);
}

static void setup_image_load_base_physical_address() {
    g_multiboot2Context.image_load_base_physical_address.enabled = TRUE;
    g_multiboot2Context.image_load_base_physical_address.load_base_addr = get_bytes(4);
}

void init_multiboot2() {
    multiboot2_info_pointer_index = 0;
    g_multiboot2Context.total_size = get_bytes(4);
    g_multiboot2Context._reserved = get_bytes(4);
    u32 type, size;
    do {
        type = get_bytes(4);
        size = get_bytes(4) - 8;

        switch (type)
        {
        case MIT_TERMINATED:
            break;
        case MIT_BOOT_COMMAND_LINE:
            setup_boot_command_line(size);
            break;
        case MIT_BOOT_LOADER_NAME:
            setup_boot_loader_name(size);
            break;
        case MIT_BASIC_MEMORY_INFORMATION:
            setup_basic_memory_information();
            break;
        case MIT_BIOS_BOOT_DEVICE:
            setup_bios_boot_device();
            break;
        case MIT_MEMORY_MAP:
            setup_memory_map(size);
            break;
        case MIT_FRAME_BUFFER_INFO:
            setup_frame_buffer_info(size);
            break;
        case MIT_ELF_SYMBOLS:
            setup_elf_symbols(size);
            break;
        case MIT_APM_TABLE:
            setup_apm_name();
            break;
        case MIT_ACPI_OLD_RSDP:
            setup_acpi_old_rsdp(size);
            break;
        case MIT_IMAGE_LOAD_BASE_PHYSICAL_ADDRESS:
            setup_image_load_base_physical_address();
            break;
        default:
            unknown_type(type);
            get_bytes(size);
        }
        align(8);
    } while (type != MIT_TERMINATED);
}
