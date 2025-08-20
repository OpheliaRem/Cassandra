#ifndef VGA_TERMINAL_H
#define VGA_TERMINAL_H

#include <stdint.h>
#include <stddef.h>

#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000

#define PROMPT_SYMBOL '$'
#define PROMPT_LEN 3

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

void init_terminal(void);
void terminal_set_color(uint8_t color);

void terminal_newline(void);
void terminal_backspace(void);
void terminal_clear(void);

void terminal_scroll_down(void);

void terminal_putchar(char c);
void terminal_write(const char* data);
void terminal_writeln(const char* str);
void terminal_write_int(int num);
void terminal_write_binary(int num);
void terminal_write_oct(int num);
void terminal_write_hex(int num);

char* terminal_read(void);

void terminal_print_new_prompt(void);

#endif