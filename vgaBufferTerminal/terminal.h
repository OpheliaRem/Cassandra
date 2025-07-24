#ifndef _VGA_TERMINAL_H
#define _VGA_TERMINAL_H

#include <stdint.h>
#include <stddef.h>
#include "../string/string.h"

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

static void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

static void terminal_set_color(uint8_t color) {
	terminal_color = color;
}

static void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_newline(void) {
	terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        terminal_row = 0;
    }
}

static void terminal_putchar(char c) {
	terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_newline();
	}
}

static void terminal_backspace(void) {
	if (terminal_column == 2) {
		return;
	}

	terminal_put_entry_at(' ', terminal_color, --terminal_column, terminal_row);
}

static void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size && data[i]; i++) {
		switch (data[i]) {
			case '\n':
				terminal_newline();
				break;
			case '\b':
				terminal_backspace();
				break;
			default:
				terminal_putchar(data[i]);
				break;
		}
	}
}

static void terminal_write_string(const char* data) {
	terminal_write(data, strlen(data));
}

#define PROMPT_SYMBOL '$'

static void terminal_print_prompt(void) {
	if (terminal_column != 0) {
		terminal_newline();
	}
	terminal_putchar(PROMPT_SYMBOL);
	terminal_putchar(' ');
}

#endif