#include "general_exception_handler.h"
#include "../../../vgaBufferTerminal/terminal.h"
#include <stdbool.h>

static const char* const EXC_NAME[32] = {
    "Divide Error",               // 0
    "Debug",                      // 1
    "NMI Interrupt",              // 2
    "Breakpoint",                 // 3
    "Overflow",                   // 4
    "BOUND Range Exceeded",       // 5
    "Invalid Opcode",             // 6
    "Device Not Available",       // 7
    "Double Fault",               // 8  (errcode)
    "Coprocessor Segment Overrun",// 9  (reserved/legacy)
    "Invalid TSS",                // 10 (errcode)
    "Segment Not Present",        // 11 (errcode)
    "Stack-Segment Fault",        // 12 (errcode)
    "General Protection",         // 13 (errcode)
    "Page Fault",                 // 14 (errcode)
    "Reserved",                   // 15
    "x87 FPU Floating-Point",     // 16
    "Alignment Check",            // 17 (errcode)
    "Machine Check",              // 18
    "SIMD Floating-Point",        // 19
    "Virtualization",             // 20
    "Control Protection",         // 21 (CET)
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Security Exception",         // 30 (errcode)
    "Reserved"                    // 31
};

static inline bool vector_has_errcode(int v) {
    switch (v) {
        case 8: case 10: case 11: case 12: case 13: case 14: case 17: case 30: return true;
        default: return false;
    }
}

static void dump_registers(const Registers* r) {
    terminal_writeln("");
    terminal_writeln("---- Register dump ----");

    terminal_write("EAX="); terminal_write_hex(r->eax);
    terminal_write("  EBX="); terminal_write_hex(r->ebx);
    terminal_write("  ECX="); terminal_write_hex(r->ecx);
    terminal_write("  EDX="); terminal_write_hex(r->edx); terminal_writeln("");

    terminal_write("ESI="); terminal_write_hex(r->esi);
    terminal_write("  EDI="); terminal_write_hex(r->edi);
    terminal_write("  EBP="); terminal_write_hex(r->ebp);
    terminal_write("  ESP*="); terminal_write_hex(r->esp); terminal_writeln("");

    terminal_writeln("-----------------------");
}

void exception_handler(int vector, uint32_t error_code, Registers* regs) {
    terminal_writeln("\n=====EXCEPTION=====");

    terminal_write("Vector:");
    terminal_write_int(vector);
    terminal_write(" ["); 
    terminal_write((vector >= 0 && vector < 32) ? EXC_NAME[vector] : "Unknown"); 
    terminal_writeln("]");

    if (vector_has_errcode(vector)) {
        terminal_write("Error code: "); terminal_write_hex(error_code); terminal_writeln("");
    } else {
        terminal_writeln("Error code: (n/a for this vector)");
    }

    dump_registers(regs);

    terminal_writeln("System halted.");
    asm volatile("cli; hlt");
}