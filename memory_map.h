#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#define VRAM_MAP_ADDR       0xE0000000

#define RAM_START_VIRT_ADDR 0xC0000000

#define KHEAP_MAP_PHY_START 0x40000

#define KHEAP_START_VIRT_ADDR   (RAM_START_VIRT_ADDR + KHEAP_MAP_PHY_START)

#define TOTAL_PHY_MEMORY 0x80000

#define PAGE_SIZE 4096

#define BUFFER_PAGES 4

#define PAGES_PER_MIB 256

#define VM_SECTION_NUM 6

#define PAGE_TABLE_SIZE 0x400 //1KB

#define SIZE_MB 1048576

#endif