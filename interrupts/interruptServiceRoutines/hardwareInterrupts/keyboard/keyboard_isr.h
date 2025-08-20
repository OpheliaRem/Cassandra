#ifndef KEYBOARD_ISR
#define KEYBOARD_ISR

void __attribute__((naked)) keyboard_isr(void);
void keyboard_isr_c(void);

#endif