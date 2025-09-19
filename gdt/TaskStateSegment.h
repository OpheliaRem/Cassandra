#ifndef TASK_STATE_SEGMENT
#define TASK_STATE_SEGMENT

#include <stdint.h>

#define TSS_INDEX_IN_GDT 5

typedef struct __attribute__((packed)) {
    uint32_t prev_tss;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;   uint32_t ss1;
    uint32_t esp2;   uint32_t ss2;
    uint32_t cr3;    uint32_t eip;   uint32_t eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} TaskStateSegment;

extern TaskStateSegment tss;

void tss_init(void* kernel_stack_top);
void tss_set_kernel_stack(void* top);

#endif