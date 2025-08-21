#include "keyboard.h"
#include "../../../interruptDescriptorTable/InterruptDescriptorTableEntry.h"
#include "../../../../vgaBufferTerminal/terminal.h"
#include <stdint.h>
#include <stdbool.h>

// таблица для scancode set 1 (make codes)
static const char scancode_table[128] = {
    0,   27, '1', '2', '3', '4', '5', '6',   // 0x00–0x07
  '7', '8', '9', '0', '-', '=', '\b',        // 0x08–0x0E (0x0E = Backspace)
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u',   // 0x0F–0x16
  'i', 'o', 'p', '[', ']', '\n',             // 0x17–0x1C (0x1C = Enter)
    0,  'a', 's', 'd', 'f', 'g', 'h', 'j',   // 0x1D–0x24 (0x1D = Ctrl)
  'k', 'l', ';', '\'', '`',                  // 0x25–0x29
    0,  '\\', 'z', 'x', 'c', 'v', 'b', 'n',  // 0x2A–0x31 (0x2A = LShift)
  'm', ',', '.', '/', 0,                     // 0x32–0x36 (0x36 = RShift)
  '*',   0,  ' ',   0,                       // 0x37–0x3A (0x39 = Space)
    0,    0,    0,    0,    0,    0,    0,   // 0x3B–0x41 (F1–F7)
    0,    0,    0,    0,    0,    0,         // 0x42–0x47 (F8–F12)
    0,    0,    0,    0,    0,    0,    0,   // 0x48–0x4E
    0,    0,    0,    0,    0,    0,    0,   // 0x4F–0x55
    0,    0,    0,    0,    0,    0,    0    // 0x56–0x7F
};

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline uint8_t keyboard_get_status() {
    return inb(KEYBOARD_STATUS_PORT);
}

static inline uint8_t keyboard_get_data() {
    return inb(KEYBOARD_CONTROLLER);
}

static inline void determine_action(char c) {
    switch (c)
    {
    case '\n':
      return terminal_execute_command();
    case '\b':
      return terminal_backspace();
    default:
      return terminal_putchar(c);
    }
}

void __attribute__((naked)) keyboard_isr(void) {
    asm volatile(
        "pushal\n"                  // сохранить все регистры
        "call keyboard_isr_c\n"     // вызвать C-функцию обработки
        "popal\n"                   // восстановить регистры
        "iret\n"                    // вернуть управление процессору
    );
}

void keyboard_isr_c(void) {

    uint8_t scancode = keyboard_get_data();

    pic_send_eoi(KEYBOARD_IRQ);

    if (scancode & 0x80) {
        return;
    }

    char c = scancode_table[scancode];

    determine_action(c);
}

void keyboard_init(void) {
    idt_set_descriptor(KEYBOARD_INTERRUPT_VECTOR, keyboard_isr, 0x08, 0x8E);
    pic_set_mask(KEYBOARD_IRQ, false);
}
