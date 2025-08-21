#include "pit.h"
#include <stdint.h>
#include "../../../programmableInterruptController/PIC8259.h"
#include "../../../interruptDescriptorTable/idt_initialization.h"
#include "../../../interruptDescriptorTable/InterruptDescriptorTableEntry.h"

static volatile uint64_t g_ticks = 0;
static uint32_t g_freq = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

void __attribute__((naked)) pit_handler(void) {
    asm volatile(
        "pusha\n"
        "cld\n"
        "call pit_handler_c\n"
        "popa\n"
        "iret\n"
    );
}

void pit_handler_c(void) {
    g_ticks++;

    pic_send_eoi(PIT_INTERRUPT_VECTOR - PIC1_OFFSET);
}

uint64_t pit_get_ticks(void) {
    return g_ticks;
}

uint32_t pit_get_frequency(void) {
    return g_freq;
}

static void pit_set_frequency(uint32_t desired_hz) {
    if (desired_hz == 0) {
        desired_hz = 100;
    }

    uint32_t divisor = PIT_BASE_FREQUENCY / desired_hz;

    if (divisor == 0) {
        divisor = 1;
    }

    if (divisor > 0xFFFF) {
        divisor = 0xFFFF;
    }

    g_freq = PIT_BASE_FREQUENCY / divisor;

    outb(MODE_COMMAND_REGISTER, PIT_CMD_CHANNEL0_LH_MODE2);
    outb(CHANNEL0_DATA_PORT, (uint8_t)(divisor & 0xFF));
    outb(CHANNEL0_DATA_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

void pit_init(uint32_t desired_hz) {
    idt_set_descriptor(PIT_INTERRUPT_VECTOR, pit_handler, 0x08, 0x8E);

    pit_set_frequency(desired_hz);

    pic_set_mask(PIT_IRQ, false);
}

void pit_sleep_ms(uint64_t ms) {
    if (g_freq == 0) {
        return;
    }

    uint64_t need = (ms * (uint64_t)g_freq + 999) / 1000;

    uint64_t target = g_ticks + need;

    while (g_ticks < target) {
        __asm__ __volatile__("hlt");
    }
}