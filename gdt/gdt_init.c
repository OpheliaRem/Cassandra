#include "gdt_init.h"
#include "GdtrModel.h"
#include "GdtEntry.h"

static void set_gdt_entry(GlobalDescriptorTableEntry* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
    entry->limit_low    = (uint16_t)(limit & 0xFFFF);
    entry->base_low     = (uint16_t)(base & 0xFFFF);
    entry->base_middle  = (uint8_t)((base >> 16) & 0xFF);
    entry->access       = access;
    entry->granularity  = (uint8_t)(((limit >> 16) & 0x0F) | (flags & 0xF0));
    entry->base_high    = (uint8_t)((base >> 24) & 0xFF);
}

void init_gdt() {
    set_gdt_entry(&global_descriptor_table[0], 0, 0, 0, 0); // NULL
    set_gdt_entry(&global_descriptor_table[1], 0, 0xFFFFF, 0x9A, 0xC0); // Code
    set_gdt_entry(&global_descriptor_table[2], 0, 0xFFFFF, 0x92, 0xC0); // Data

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
}

