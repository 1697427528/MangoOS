#ifndef __MULTIBOOT2_H__
#define __MULTIBOOT2_H__

enum Multiboot2InfoType {
    MIT_TERMINATED = 0,
    MIT_BOOT_COMMAND_LINE = 1,
    MIT_BOOT_LOADER_NAME = 2,
    MIT_BASIC_MEMORY_INFORMATION = 4,
    MIT_BIOS_BOOT_DEVICE = 5,
    MIT_MEMORY_MAP = 6,
    MIT_FRAME_BUFFER_INFO = 8,
    MIT_ELF_SYMBOLS = 9,
    MIT_APM_TABLE = 10,
    MIT_ACPI_OLD_RSDP = 14,
    MIT_IMAGE_LOAD_BASE_PHYSICAL_ADDRESS = 21,
};

typedef struct _BootCommandLine {
    bool enabled;
    u8 length;
    char* string;
} BootCommandLine;

typedef struct _BootLoaderName {
    bool enabled;
    u8 length;
    char* string;
} BootLoaderName;

typedef struct _BasicMemoryInformation {
    bool enabled;
    u32 mem_lower;
    u32 mem_upper;
} BasicMemoryInformation;

typedef struct _BIOSBootDevice {
    bool enabled;
    u32 biosdev;
    u32 partition;
    u32 sub_partition;
} BIOSBootDevice;

enum MemoryEntryType {
    MET_AVAILABLE = 1,
    MET_RESERVED = 2,
    MET_ACPI_RECLAIMABLE = 3,
    MET_NVS = 4,
    MET_BADRAM = 5,
};

typedef struct _MemoryEntry {
    u64 base_addr;
    u64 length;
    u32 type;
    u32 _reserved;
} MemoryEntry;

typedef struct _MemoryMap {
    bool enabled;
    u32 entry_size;
    u32 entry_version;
    u32 length;
    MemoryEntry* entries;
} MemoryMap;

enum FrameBufferType {
    FBT_INDEXED = 0,
    FBT_RGB = 1,
    FBT_EGA_TEXT = 2,
};

typedef struct _FrameBufferPalette {
    u8 red_value;
    u8 blue_value;
    u8 green_value;
} FrameBufferPalette;

typedef union _FrameBufferColorInfo {
    struct IndexedColorInfo {
        u32 framebuffer_palette_num_colors;
        FrameBufferPalette* framebuffer_palettes;
    } idx_color_info;
    struct RGBColorInfo {
        u8 framebuffer_red_field_position;
        u8 framebuffer_red_mask_size;
        u8 framebuffer_green_field_position;
        u8 framebuffer_green_mask_size;
        u8 framebuffer_blue_field_position;
        u8 framebuffer_blue_mask_size;
    } rgb_color_info;
} FrameBufferColorInfo;

typedef struct _FrameBufferInfo {
    u64 framebuffer_addr;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    u16 _reserved;
    FrameBufferColorInfo color_info;
} FrameBufferInfo;

typedef struct _ELFSymbols {
    bool enabled;
    u16 num;
    u16 entsize;
    u16 shndx;
    u16 _reserved;
    u32 length;
    char* section_headers;
} ELFSymbols;

typedef struct _APMTable {
    bool enabled;
    u16 version;
    u16 cseg;
    u32 offset;
    u16 cseg_16;
    u16 dseg;
    u16 flags;
    u16 cseg_len;
    u16 cseg_16_len;
    u16 dseg_len;
} APMTable;

typedef struct _ACPIOldRSDP {
    bool enabled;
    u32 length;
    u8* copy_of_rsdpv1;
} ACPIOldRSDP;

typedef struct _ImageLoadBasePhysicalAddress {
    bool enabled;
    u32 load_base_addr;
} ImageLoadBasePhysicalAddress;

typedef struct _Multiboot2Context {
    u32 total_size;
    u32 _reserved;
    BootCommandLine boot_cammand_line;
    BootLoaderName boot_loader_name;
    BasicMemoryInformation basic_memory_information;
    BIOSBootDevice bios_boot_device; 
    MemoryMap memory_map;
    FrameBufferInfo frame_buffer_info;
    ELFSymbols elf_symbols;
    APMTable apm_table;
    ACPIOldRSDP acpi_old_rsdp;
    ImageLoadBasePhysicalAddress image_load_base_physical_address;
} Multiboot2Context;

void init_multiboot2();

#endif
