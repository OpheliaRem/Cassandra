#include "terminal.h"
#include "../innerStd//string.h"
#include "../innerStd/StackVoidPtr.h"
#include "../innerStd/allocator.h"
#include "../innerStd/convert.h"

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

static StackVoidPtr terminal_scroll_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static void fill_row_with(size_t y, unsigned char c) {
	if (y >= VGA_HEIGHT) {
		return;
	}

	for (size_t x = 0; x < VGA_WIDTH; ++x) {
		const size_t index = y * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(c, terminal_color);
	}
}

static void fill_whole_buffer_with(unsigned char c) {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(c, terminal_color);
		}
	}
}

void init_terminal(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	fill_whole_buffer_with(' ');

	stack_void_ptr_init(&terminal_scroll_buffer);
}

void terminal_set_color(uint8_t color) {
	terminal_color = color;
}

static void put_entry_at(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll_down() {
	uint16_t* first_line = (uint16_t*)allocate(VGA_WIDTH * sizeof(uint16_t));
	if (first_line) {
		for (int i = 0; i < VGA_WIDTH; ++i) {
			first_line[i] = terminal_buffer[i];
		}
		stack_void_ptr_push(&terminal_scroll_buffer, first_line);
	}

	for (int i = 0; i + 1 < VGA_HEIGHT; ++i) {
		for (int j = 0; j < VGA_WIDTH; ++j) {
			terminal_buffer[i * VGA_WIDTH + j] = terminal_buffer[(i + 1) * VGA_WIDTH + j];
		}
	}

	fill_row_with(VGA_HEIGHT - 1, ' ');
}

void terminal_newline(void) {
	terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        terminal_row--;
		terminal_scroll_down();
    }
}

void terminal_putchar(char c) {
	put_entry_at(c, terminal_color, terminal_column, terminal_row);

	if (++terminal_column == VGA_WIDTH) {
		terminal_newline();
	}
}

static void previous_line() {
	if (terminal_row == 0) {
		return;
	}

	--terminal_row;
	terminal_column = VGA_WIDTH - 1;
}

void terminal_backspace(void) {
	if (terminal_column == 0) {
		if (terminal_row == 0) {
			return;
		}
		previous_line();
	}

	put_entry_at(' ', terminal_color, --terminal_column, terminal_row);
}

void terminal_clear(void) {
	terminal_row = 0;
	terminal_column = 0;
	
	fill_whole_buffer_with(' ');

	stack_void_ptr_foreach(&terminal_scroll_buffer, free);
	stack_void_ptr_free(&terminal_scroll_buffer);
	stack_void_ptr_init(&terminal_scroll_buffer);
}


static void write_n_chars(const char* data, size_t size) {
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
	write_n_chars(data, strlen(data));
}

void terminal_writeln(const char* str) {
    terminal_write(str);
    terminal_newline();
}

void terminal_write_int(int num) {
	char* num_str = int_to_string(num);
	if (num_str) {
		terminal_write(num_str);
		free(num_str);
	}
}

void terminal_write_binary(int num) {
	char* num_str = int_to_string_binary(num);
	if (num_str) {
		terminal_write(num_str);
		free(num_str);
	}
}

void terminal_write_oct(int num) {
	char* num_str = int_to_string_oct(num);
	if (num_str) {
		terminal_write(num_str);
		free(num_str);
	}
}

void terminal_write_hex(int num) {
	char* num_str = int_to_string_hex(num);
	if (num_str) {
		terminal_write(num_str);
		free(num_str);
	}
}

static void print_prompt(void) {
	terminal_putchar(PROMPT_SYMBOL);
	terminal_putchar(':');
	terminal_putchar(' ');
}

void terminal_print_new_prompt(void) {
	if (terminal_column != 0) {
		terminal_newline();
	}
	print_prompt();
}