#include "general_exception_handler.h"

void exception_handler(int vector, uint32_t error_code, Registers* regs) {
    (void)vector;
    (void)error_code;
    (void)regs;
    
    stop_interrupts();
    stop_processor();
}