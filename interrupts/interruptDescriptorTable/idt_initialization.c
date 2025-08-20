#include "idt_initialization.h"
#include "InterruptDescriptorTableEntry.h"
#include "IdtrModel.h"
#include "../devices/keyboard.h"
#include "../programmableInterruptController/PIC8259.h"
#include <stdint.h>

extern void* isr_stubs[];
extern void keyboard_isr(void);

static void idt_set_descriptor(uint8_t interrupt_vector, void* isr_address, uint16_t selector, uint8_t flags) {
    InterruptDescriptorTableEntry* gate = &interrupt_descriptor_table[interrupt_vector];

    gate->interrupt_service_routine_address_low_bytes = (uint32_t)isr_address & 0xFFFF;
    gate->interrupt_service_routine_address_high_bytes = (uint32_t)isr_address >> 16;
    gate->selector = selector;
    gate->flags = flags;
    gate->reserved = 0;
}

void init_idt() {
    idtr.base = (uintptr_t)&interrupt_descriptor_table[0];
    idtr.limit = (uint16_t)sizeof(interrupt_descriptor_table) - 1;

    //exceptions
    for (int i = 0; i < 32; ++i) {
        idt_set_descriptor(i, isr_stubs[i], 0x08, 0x8E);
    }

    //keyboard
    idt_set_descriptor(KEYBOARD_INTERRUPT_VECTOR, keyboard_isr, 0x08, 0x8E);

    pic_init(PIC1_OFFSET, PIC2_OFFSET, 0xFD, 0xFF);

    asm volatile("lidt %0" : : "m"(idtr));
    
    asm volatile("sti");
}