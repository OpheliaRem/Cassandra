#ifndef DEVICE_KEYBOARD_H
#define DEVICE_KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_INTERRUPT_VECTOR 33
#define KEYBOARD_CONTROLLER 0x60
#define KEYBOARD_STATUS_PORT 0x64

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

uint8_t keyboard_get_status();
uint8_t keyboard_get_data();

#endif