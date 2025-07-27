#if !defined(__i386__)
#error "The kernel shall be compiled with a ix86-elf compiler"
#endif

#include "vgaBufferTerminal/terminal.h"
#include "interrupts/interruptDescriptorTable/idt_initialization.h"
#include "innerStd/allocator.h"
#include "innerStd/convert.h"
#include "innerStd/string.h"

void division_by_zero() {
	volatile int a = 10 / 0;
}

void kernel_main(void) {
	initialize_idt();

	terminal_initialize();
	initialize_heap();

	terminal_print_new_prompt();

	char* str = int_to_string(-100);
	if (str) {
		terminal_write(str);
		free(str);
	}

	while(1);
}