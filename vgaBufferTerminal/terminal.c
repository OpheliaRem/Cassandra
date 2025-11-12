#include "terminal.h"
#include "../innerStd//string.h"
#include "../innerStd/dataStructures/Stack.h"
#include "../innerStd/allocator.h"
#include "../innerStd/convert.h"
#include "../commandHandling/command_handling.h"

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

static inline char vga_get_char(uint16_t cell) {
	return (char)(cell & 0xFF);
}

static inline uint8_t vga_get_color(uint16_t cell) {
	return (uint8_t)(cell >> 8);
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

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void set_cursor(size_t row, size_t col) {
	uint16_t pos = row * VGA_WIDTH + col;

	outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void previous_line() {
	if (terminal_row == 0) {
		return;
	}
	--terminal_row;
	terminal_column = VGA_WIDTH;
}

static void set_index_back(void) {
	if (terminal_column == 0) {
		if (terminal_row == 0) {
			return;
		}
		previous_line();
	}

	--terminal_column;

	set_cursor(terminal_row, terminal_column);
}

static void set_index_forward(void) {
	if (++terminal_column == VGA_WIDTH) {
		terminal_newline();
	}

	set_cursor(terminal_row, terminal_column);
}

void init_terminal(void) {
	set_cursor(0, 0);

	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	fill_whole_buffer_with(' ');
}

void terminal_set_color(uint8_t color) {
	terminal_color = color;
}

static void put_entry_at(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_scroll_down() {
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

	set_cursor(terminal_row, terminal_column);
}

static void put_entry_and_move_index_forward(char c) {
	put_entry_at(c, terminal_color, terminal_column, terminal_row);
	set_index_forward();
}

static void determine_action_with_char(char c) {
	switch(c) {
		case '\n': 	return terminal_newline();
		case '\b':	return terminal_backspace();
		default:	return put_entry_and_move_index_forward(c);
	}
}

void terminal_putchar(char c) {
	determine_action_with_char(c);
}

int terminal_getchar() {
	char character = vga_get_char(terminal_buffer[terminal_row * VGA_WIDTH + terminal_column]);

	set_index_forward();

	return (int)character;
}

static void backspace(void) {
	set_index_back();

	put_entry_at(' ', terminal_color, terminal_column, terminal_row);
}

void terminal_backspace(void) {
	const size_t index = terminal_row * VGA_WIDTH + terminal_column;

	uint16_t* pos = &terminal_buffer[index];

	if (vga_get_char(*(pos - PROMPT_LEN)) == PROMPT_SYMBOL) {
		return;
	}

	backspace();
}

void terminal_clear(void) {
	terminal_row = 0;
	terminal_column = 0;
	
	fill_whole_buffer_with(' ');

	set_cursor(terminal_row, terminal_column);
}

static void write_n_chars(const char* data, size_t n) {
	for (size_t i = 0; i < n && data[i]; i++) {
		terminal_putchar(data[i]);
	}
}

void terminal_write(const char* data) {
	write_n_chars(data, string_len(data));
}

void terminal_writeln(const char* str) {
    terminal_write(str);
    terminal_newline();
}

void terminal_write_int(int num) {
	char* num_str = int_to_string(num);
	if (num_str) {
		terminal_write(num_str);
		deallocate(num_str);
	}
}

void terminal_write_binary(int num) {
	char* num_str = int_to_string_binary(num);
	if (num_str) {
		terminal_write(num_str);
		deallocate(num_str);
	}
}

void terminal_write_oct(int num) {
	char* num_str = int_to_string_oct(num);
	if (num_str) {
		terminal_write(num_str);
		deallocate(num_str);
	}
}

void terminal_write_hex(int num) {
	char* num_str = int_to_string_hex(num);
	if (num_str) {
		terminal_write(num_str);
		deallocate(num_str);
	}
}

char* terminal_read() {
    size_t current_index = terminal_row * VGA_WIDTH + terminal_column;
    //uint16_t* pos = &terminal_buffer[--current_index];
    uint16_t* end = &terminal_buffer[--current_index];

    /* while (pos != (uint16_t*)VGA_MEMORY && vga_get_char(*pos) != PROMPT_SYMBOL) {
        pos--;
    }
	pos += PROMPT_LEN;

    if (pos == (uint16_t*)VGA_MEMORY) {
        return NULL;
    } */

	while (
		&terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] != (uint16_t*)VGA_MEMORY && 
		vga_get_char(terminal_buffer[terminal_row * VGA_WIDTH + terminal_column]) != PROMPT_SYMBOL
	) {
		set_index_back();
	}

	for (int i = 0; i < PROMPT_LEN; ++i) {
		set_index_forward();
	}
	uint16_t* pos = &terminal_buffer[terminal_row * VGA_WIDTH + terminal_column];

    size_t len = end - pos + 1;

    char* command = (char*)allocate((len + 1) * sizeof(char));
    if (!command) {
        return NULL;
    }

    for (size_t i = 0; i < len; ++i) {
        //command[i] = vga_get_char(pos[i]);
		command[i] = terminal_getchar();
    }
    command[len] = '\0';

    return command;
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

void terminal_execute_command() {
	char* command = terminal_read();

	if (!command) {
		terminal_writeln("Failed reading the command. Try again.");
		return;
	}

	handle_command(command);

	deallocate(command);

	terminal_print_new_prompt();
}