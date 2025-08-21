#ifndef KEYBOARD_ISR
#define KEYBOARD_ISR

#include "../../../programmableInterruptController/PIC8259.h"

#define KEYBOARD_INTERRUPT_VECTOR 33
#define KEYBOARD_IRQ (KEYBOARD_INTERRUPT_VECTOR-PIC1_OFFSET)

#define KEYBOARD_CONTROLLER 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_init(void);

#endif