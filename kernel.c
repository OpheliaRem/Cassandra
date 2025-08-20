#if !defined(__i386__)
#error "The kernel shall be compiled with a ix86-elf compiler"
#endif

#include "gdt/gdt_init.h"
#include "vgaBufferTerminal/terminal.h"
#include "interrupts/interruptDescriptorTable/idt_initialization.h"
#include "innerStd/allocator.h"
#include "innerStd/convert.h"
#include "innerStd/string.h"

void kernel_main(void) {
	asm volatile("cli");

	init_gdt();

	init_idt();

	init_heap();
	
	init_terminal();

	terminal_print_new_prompt();

	while(1);
}