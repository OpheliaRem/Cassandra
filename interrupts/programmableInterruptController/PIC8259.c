#include "PIC8259.h"

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void pic_init(int offset1, int offset2, uint8_t mask1, uint8_t mask2) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();

    outb(PIC1_DATA, 1 << CASCADE_IRQ);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_set_mask(uint8_t irq, bool masked) {
    if (irq > 15) {
        return; // allowed only  IRQ0–IRQ15
    }

    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    uint8_t bit   = (irq < 8) ? irq : irq - 8;

    uint8_t mask = inb(port);

    if (masked) {
        mask |= (1 << bit);    // 1 = prohibit IRQ
    } else {
        mask &= ~(1 << bit);   // 0 = allow IRQ
    }

    outb(port, mask);
}