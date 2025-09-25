#include "TaskStateSegment.h"
#include "GdtEntry.h"
#include "gdt_init.h"

TaskStateSegment tss = {0};

void tss_init(void* kernel_stack_top) {
    tss.ss0 = KERNEL_DATA * 8;
    tss.esp0 = (uint32_t)kernel_stack_top;
    tss.iomap_base = sizeof(TaskStateSegment);
}

void tss_set_kernel_stack(void* top) {
    tss.esp0 = (uint32_t)top;
}
