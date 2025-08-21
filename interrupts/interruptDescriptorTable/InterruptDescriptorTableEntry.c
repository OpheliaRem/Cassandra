#include "InterruptDescriptorTableEntry.h"

void idt_set_descriptor(uint8_t interrupt_vector, void* isr_address, uint16_t selector, uint8_t flags) {
    InterruptDescriptorTableEntry* gate = &interrupt_descriptor_table[interrupt_vector];

    gate->interrupt_service_routine_address_low_bytes = (uint32_t)isr_address & 0xFFFF;
    gate->interrupt_service_routine_address_high_bytes = (uint32_t)isr_address >> 16;
    gate->selector = selector;
    gate->flags = flags;
    gate->reserved = 0;
}