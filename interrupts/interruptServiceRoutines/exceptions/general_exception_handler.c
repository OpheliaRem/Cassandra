#include "general_exception_handler.h"
#include "../../../vgaBufferTerminal/terminal.h"

void exception_handler(int vector, uint32_t error_code, Registers* regs) {
    (void)vector;
    (void)error_code;
    (void)regs;

    terminal_write("An exception has occured");
    
    stop_interrupts();
    while (1) stop_processor();
}