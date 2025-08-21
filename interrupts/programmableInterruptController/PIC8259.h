#ifndef PIC8259_FILE
#define PIC8259_FILE

#include <stdint.h>
#include <stdbool.h>

#define PIC1            0x20
#define PIC2            0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)

#define PIC_EOI 0x20

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

#define CASCADE_IRQ 2

#define PIC1_OFFSET 32
#define PIC2_OFFSET 40

void pic_send_eoi(uint8_t irq);
void pic_init(int offset1, int offset2, uint8_t mask1, uint8_t mask2);
void pic_set_mask(uint8_t irq, bool masked);

#endif