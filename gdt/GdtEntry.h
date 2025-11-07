#ifndef GDT_ENTRY_FILE
#define GDT_ENTRY_FILE

#include <stdint.h>

typedef struct __attribute__((packed)) GlobalDescriptorTableEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high; 
} GlobalDescriptorTableEntry;

extern GlobalDescriptorTableEntry global_descriptor_table[];

#endif