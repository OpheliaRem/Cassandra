#include "idt_initialization.h"
#include "InterruptDescriptorTableEntry.h"
#include "IdtrModel.h"
#include "../interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard.h"
#include "../interruptServiceRoutines/hardwareInterrupts/programmableIntervalTimer/pit.h"
#include "../programmableInterruptController/PIC8259.h"
#include <stdint.h>

#include "../../vgaBufferTerminal/terminal.h"

extern void* isr_stubs[];

extern void isr80_stub(void);

InterruptDescriptorTableEntry interrupt_descriptor_table[256];

void dump_idt80(void) {
    InterruptDescriptorTableEntry *e = &interrupt_descriptor_table[0x80];
    uint32_t low  = e->interrupt_service_routine_address_low_bytes;
    uint32_t high = e->interrupt_service_routine_address_high_bytes;
    uint32_t off  = (high << 16) | low;
    terminal_write("IDT[0x80]:\n");
    terminal_write("  offset_low="); terminal_write_hex((uint16_t)low);
    terminal_write("  selector=");    terminal_write_hex(e->selector);
    terminal_write("  flags=");       terminal_write_hex(e->flags);
    terminal_write("  offset_high="); terminal_write_hex((uint16_t)high);
    terminal_write("\n  composed offset="); terminal_write_hex(off);
    terminal_write("\n  &isr80_stub="); terminal_write_hex((uint32_t)isr80_stub);
    terminal_write("\n");
}

void init_idt() {
    idtr.base = (uintptr_t)&interrupt_descriptor_table[0];
    idtr.limit = (uint16_t)sizeof(interrupt_descriptor_table) - 1;

    //exceptions
    for (int i = 0; i < 32; ++i) {
        idt_set_descriptor(i, isr_stubs[i], 0x08, 0x8E);
    }

    pic_init(PIC1_OFFSET, PIC2_OFFSET, 0xFF, 0xFF);

    keyboard_init();
    pit_init(18);

    // selector = 0x08 (kernel code segment)
    // flags = 0xEE = 1110 1110b
    // P=1, DPL=3, Type=0xE
    idt_set_descriptor(0x80, isr80_stub, 0x08, 0xEE);

    asm volatile("lidt %0" : : "m"(idtr));
    
    asm volatile("sti");
}