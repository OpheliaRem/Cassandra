#ifndef GDTR_MODEL
#define GDTR_MODEL

#include <stdint.h>

typedef struct __attribute__((packed)) GdtrModel {
    uint16_t limit;
    uint32_t base; 
} GdtrModel;

static GdtrModel gdtr;

#endif