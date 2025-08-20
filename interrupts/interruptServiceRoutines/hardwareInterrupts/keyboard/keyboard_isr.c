#include "keyboard_isr.h"
#include "../../../programmableInterruptController/PIC8259.h"
#include "../../../interruptDescriptorTable/idt_initialization.h"
#include "../../../devices/keyboard.h"
#include "../../../../vgaBufferTerminal/terminal.h"
#include "../../../../commandHandling/command_handling.h"
#include "../../../../innerStd/allocator.h"
#include <stdint.h>

static void determine_action(char c) {
    switch (c)
    {
    case '\n':
      char* command = terminal_read();
      handle_command(command);
      return free(command);
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

    pic_send_eoi(KEYBOARD_INTERRUPT_VECTOR - PIC1_OFFSET);

    if (scancode & 0x80) {
        return;
    }

    char c = scancode_table[scancode];

    determine_action(c);
}
