#if !defined(__i386__)
#error "The kernel shall be compiled with a ix86-elf compiler"
#endif

#include "gdt/gdt_init.h"
#include "interrupts/interruptDescriptorTable/idt_initialization.h"
#include "interrupts/interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard.h"
#include "commandHandling/command_handling.h"
#include "vgaBufferTerminal/terminal.h"
#include "innerStd/allocator.h"

void memcpy(uint8_t* dst, uint8_t* src, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dst[i] = src[i];
	}
}

void kernel_main(void) {
	init_heap();
	
	init_terminal();

	init_gdt();

	init_idt();


	init_command_handling();

	terminal_print_new_prompt();

	//EXPERIMENTAL

	extern uint8_t _binary_hello_bin_start[];
	extern uint8_t _binary_hello_bin_end[];

	size_t size = _binary_hello_bin_end - _binary_hello_bin_start + 1;

	uint8_t* program_address = (uint8_t*)allocate(size);

	memcpy(program_address, _binary_hello_bin_start, size);

	void(*user_process)(void) = (void(*)(void))program_address;
	user_process();

	free(program_address);

	//EXPERIMENTAL_END

	while(1) {
		asm volatile("hlt");
		keyboard_provide_command_input();
	}
}