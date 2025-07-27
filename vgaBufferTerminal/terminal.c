#include "terminal.h"
#include "../innerStd//string.h"

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(void) {
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

void terminal_set_color(uint8_t color) {
	terminal_color = color;
}

static void terminal_put_entry_at(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_newline(void) {
	terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        terminal_row = 0;
    }
}

void terminal_putchar(char c) {
	terminal_put_entry_at(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_newline();
	}
}

void terminal_backspace(void) {
	if (terminal_column == FIRST_COLUMN_FOR_TEXT) {
		return;
	}

	terminal_put_entry_at(' ', terminal_color, --terminal_column, terminal_row);
}

static void terminal_write_n_chars(const char* data, size_t size) {
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

void terminal_write(const char* data) {
	terminal_write_n_chars(data, strlen(data));
}

void terminal_writeln(const char* str) {
    terminal_write(str);
    terminal_newline();
}

static void terminal_print_prompt(void) {
	terminal_putchar(PROMPT_SYMBOL);
	terminal_putchar(':');
	terminal_putchar(' ');
}

void terminal_print_new_prompt(void) {
	if (terminal_column != 0) {
		terminal_newline();
	}
	terminal_print_prompt();
}