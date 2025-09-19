#include "TaskStateSegment.h"
#include "GdtEntry.h"
#include "gdt_init.h"

TaskStateSegment tss;

#define SEL(index)   ((uint16_t)((index) << 3))
#define RPL_USER     0x3

static const uint16_t TSS_SELECTOR = SEL(TSS_INDEX_IN_GDT); // 5 << 3 = 0x28
static const uint16_t KERNEL_DS_SEL = SEL(KERNEL_DATA);     // 2 << 3 = 0x10

void tss_init(void* kernel_stack_top) {
    for (unsigned int i = 0; i < sizeof(tss); ++i) {
        ((uint8_t*)&tss)[i] = 0;
    }

    tss.ss0 = KERNEL_DS_SEL;
    tss.esp0 = (uint32_t)kernel_stack_top;

    tss.iomap_base = sizeof(tss);

    asm volatile("ltr %0" : : "r" ((uint16_t)TSS_SELECTOR));
}

void tss_set_kernel_stack(void* top) {
    tss.esp0 = (uint32_t)top;
}
