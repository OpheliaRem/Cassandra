#include "idt_initialization.h"
#include "InterruptDescriptorTableEntry.h"
#include "IdtrModel.h"
#include "../interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard.h"
#include "../interruptServiceRoutines/hardwareInterrupts/programmableIntervalTimer/pit.h"
#include "../programmableInterruptController/PIC8259.h"
#include <stdint.h>

extern void* isr_stubs[];

InterruptDescriptorTableEntry interrupt_descriptor_table[256];

void init_idt() {
    idtr.base = (uintptr_t)&interrupt_descriptor_table[0];
    idtr.limit = (uint16_t)sizeof(interrupt_descriptor_table) - 1;

    //exceptions
    for (int i = 0; i < 32; ++i) {
        idt_set_descriptor(i, isr_stubs[i], 0x08, 0x8E);
    }

    pic_init(PIC1_OFFSET, PIC2_OFFSET, 0xFF, 0xFF);

    keyboard_init();
    pit_init(1000);

    asm volatile("lidt %0" : : "m"(idtr));
    
    asm volatile("sti");
}