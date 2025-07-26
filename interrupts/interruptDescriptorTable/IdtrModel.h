#ifndef IDTR_MODEL
#define IDTR_MODEL

#include <stdint.h>

typedef struct IdtrModel {
    uint16_t limit;
    uint32_t base;
} IdtrModel;

static IdtrModel idtr;

#endif