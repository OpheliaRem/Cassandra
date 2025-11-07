#if !defined(__i386__)
#error "The kernel shall be compiled with a ix86-elf compiler"
#endif

#include "gdt/gdt_init.h"
#include "gdt/TaskStateSegment.h"
#include "userSpace/process.h"
#include "interrupts/interruptDescriptorTable/idt_initialization.h"
#include "interrupts/interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard.h"
#include "commandHandling/command_handling.h"
#include "vgaBufferTerminal/terminal.h"
#include "innerStd/allocator.h"
#include "innerStd/io.h"

void memcpy(uint8_t* dst, uint8_t* src, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dst[i] = src[i];
	}
}

void* allocate_user_memory(size_t size) {
	(void)size;
    return (void*)0x400000;
}

void kernel_main(void) {
	init_gdt();

	init_idt();

	void* kernel_stack_top = allocate(4096) + 4096;
	
	tss_init(kernel_stack_top);

	init_heap();
	
	init_terminal();

	init_io_dispatcher(VGA_BUFFER_IN, VGA_BUFFER_OUT);

	init_command_handling();

	terminal_print_new_prompt();

	/* //EXPERIMENTAL
	extern uint8_t _binary_hello_bin_start[];
    extern uint8_t _binary_hello_bin_end[];
    
    size_t code_size = _binary_hello_bin_end - _binary_hello_bin_start;
    
    uint8_t* user_code = (uint8_t*)allocate_user_memory(code_size);
    memcpy(user_code, _binary_hello_bin_start, code_size);
    
    uint8_t* user_stack = (uint8_t*)allocate_user_memory(4096) + 4096;
    
    switch_to_user_mode(user_code, user_stack);
	//EXPERIMENTAL_END */

	while(1) {
		asm volatile("hlt");
		keyboard_provide_command_input();
	}
}