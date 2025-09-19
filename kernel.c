#if !defined(__i386__)
#error "The kernel shall be compiled with a ix86-elf compiler"
#endif

#include "gdt/gdt_init.h"
#include "interrupts/interruptDescriptorTable/idt_initialization.h"
#include "interrupts/interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard.h"
#include "commandHandling/command_handling.h"
#include "vgaBufferTerminal/terminal.h"
#include "innerStd/allocator.h"

extern void enter_user_mode(uint32_t, uint32_t);

void memcpy(uint8_t* dst, uint8_t* src, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dst[i] = src[i];
	}
}

void kernel_main(void) {
	init_gdt();

	init_idt();

	init_heap();
	
	init_terminal();

	init_command_handling();

	terminal_print_new_prompt();

	//EXPERIMENTAL

	extern uint8_t _binary_hello_bin_start[];
	extern uint8_t _binary_hello_bin_end[];

	#define USER_PROG_ADDR 0x00400000
	#define USER_STACK_TOP 0x00402000  // стек чуть выше программы

	size_t size = _binary_hello_bin_end - _binary_hello_bin_start;
	memcpy((uint8_t*)USER_PROG_ADDR, _binary_hello_bin_start, size);

	enter_user_mode(USER_PROG_ADDR, USER_STACK_TOP);

	//EXPERIMENTAL_END


	while(1) {
		asm volatile("hlt");
		keyboard_provide_command_input();
	}
}