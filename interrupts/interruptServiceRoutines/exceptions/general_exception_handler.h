#ifndef GENERAL_EXCEPTION_HANDLER
#define GENERAL_EXCEPTION_HANDLER

#include <stdint.h>

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
} Registers;

extern void stop_processor(void);
extern void stop_interrupts(void);

void exception_handler(int vector, uint32_t error_code, Registers* regs);

#endif