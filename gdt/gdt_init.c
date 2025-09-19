#include "gdt_init.h"
#include "GdtrModel.h"
#include "GdtEntry.h"
#include "TaskStateSegment.h"

#define KSTACK_SIZE 4096
static uint8_t kernel_stack[KSTACK_SIZE] __attribute__((aligned(16)));

#define GDT_LIMIT 0xFFFFF

GlobalDescriptorTableEntry global_descriptor_table[6];
GdtrModel gdtr;

static void set_gdt_entry(GlobalDescriptorTableEntry* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry->limit_low    = (uint16_t)(limit & 0xFFFF);
    entry->base_low     = (uint16_t)(base & 0xFFFF);
    entry->base_middle  = (uint8_t)((base >> 16) & 0xFF);
    entry->access       = access;
    entry->granularity  = (uint8_t)(((limit >> 16) & 0x0F) | (flags & 0xF0));
    entry->base_high    = (uint8_t)((base >> 24) & 0xFF);
}

static void set_tss_entry(int index, uint32_t base, uint32_t limit) {
    set_gdt_entry(&global_descriptor_table[index], base, limit, 0x89, 0x00);
}

void init_gdt() {
    set_gdt_entry(&global_descriptor_table[NULL_SEGMENT], 0, 0, 0, 0); // NULL

    //kernel_space
    set_gdt_entry(&global_descriptor_table[KERNEL_CODE], 0, 0xFFFFF, 0x9A, 0xC0); // Code
    set_gdt_entry(&global_descriptor_table[KERNEL_DATA], 0, 0xFFFFF, 0x92, 0xC0); // Data

    //user_space
    set_gdt_entry(&global_descriptor_table[USER_CODE], 0, GDT_LIMIT, 0xFA, 0xC0); //Code
    set_gdt_entry(&global_descriptor_table[USER_DATA], 0, GDT_LIMIT, 0xF2, 0xC0); //Data

    //tss
    set_tss_entry(TSS_INDEX_IN_GDT, (uint32_t)&tss, sizeof(tss) - 1);

    gdtr.limit = sizeof(global_descriptor_table) - 1;
    gdtr.base  = (uint32_t)&global_descriptor_table;

    asm volatile("lgdt (%0)" : : "r" (&gdtr));

    asm volatile (
        "ljmp $0x08, $.flush_cs\n\t"
        ".flush_cs:\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        :
        :
        : "ax"
    );

    void* top = kernel_stack + KSTACK_SIZE;
    tss_init(top);
}

