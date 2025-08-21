#ifndef INTERRUPT_DESCRIPTOR_TABLE_ENTRY
#define INTERRUPT_DESCRIPTOR_TABLE_ENTRY

#include <stdint.h>

typedef struct __attribute__((packed)) InterruptDescriptorTableEntry {
    uint16_t interrupt_service_routine_address_low_bytes;
    uint16_t selector;
    uint8_t reserved; //zero
    uint8_t flags;
    uint16_t interrupt_service_routine_address_high_bytes;
} InterruptDescriptorTableEntry;

extern InterruptDescriptorTableEntry interrupt_descriptor_table[256];

void idt_set_descriptor(uint8_t interrupt_vector, void* isr_address, uint16_t selector, uint8_t flags);

#endif