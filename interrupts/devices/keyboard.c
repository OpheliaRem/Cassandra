#include "keyboard.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

uint8_t keyboard_get_status() {
    return inb(KEYBOARD_STATUS_PORT);
}

uint8_t keyboard_get_data() {
    return inb(KEYBOARD_CONTROLLER);
}