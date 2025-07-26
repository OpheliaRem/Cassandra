#if !defined(__i386__)
#error "The kernel shall be compiled with a ix86-elf compiler"
#endif

#include "vgaBufferTerminal/terminal.h"
#include "interrupts/interruptDescriptorTable/idt_initialization.h"

void kernel_main(void) {
	terminal_initialize();
	terminal_print_new_prompt();

	initialize_idt();

	while(1);
}