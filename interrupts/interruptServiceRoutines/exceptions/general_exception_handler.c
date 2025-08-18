#include "general_exception_handler.h"
#include "../../../vgaBufferTerminal/terminal.h"

void exception_handler(int vector, uint32_t error_code, Registers* regs) {
    (void)vector;
    (void)error_code;
    (void)regs;

    terminal_write("An exception has occured\n");
    
    asm volatile("cli");
    asm volatile("hlt");
}