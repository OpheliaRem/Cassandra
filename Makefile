CROSS_BIN := $(HOME)/opt/cross/bin
CC := $(CROSS_BIN)/i686-elf-gcc
AS := $(CROSS_BIN)/i686-elf-as

all: cassandra

cassandra: kernel.o boot.o exception_asm.o exception_c.o terminal.o string.o allocator.o convert.o idt_initialization.o gdt_init.o stack.o commands.o command_handling.o parser.o pic8259.o keyboard.o keyboard_isr_c.o linked_list.o
	$(CC) -T ./_build/linker.ld -o cassandra -ffreestanding -O2 -nostdlib \
	    ./_build/boot.o \
	    ./_build/exception_asm.o \
	    ./_build/exception_c.o \
		./_build/terminal.o \
		./_build/stack.o \
		./_build/commands.o \
		./_build/command_handling.o \
		./_build/parser.o \
		./_build/string.o \
		./_build/linked_list.o \
		./_build/allocator.o \
		./_build/convert.o \
		./_build/gdt_init.o \
		./_build/idt_initialization.o \
		./_build/pic8259.o \
		./_build/keyboard.o \
		./_build/keyboard_isr_c.o \
	    ./_build/kernel.o \
	    -lgcc

kernel.o: kernel.c
	$(CC) -c kernel.c -o ./_build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

idt_initialization.o: ./interrupts/interruptDescriptorTable/idt_initialization.c
	$(CC) -c $< -o ./_build/idt_initialization.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gdt_init.o: ./gdt/gdt_init.c
	$(CC) -c $< -o ./_build/gdt_init.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

convert.o: ./innerStd/convert.c
	$(CC) -c $< -o ./_build/convert.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

allocator.o: ./innerStd/allocator.c
	$(CC) -c $< -o ./_build/allocator.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

string.o: ./innerStd/string.c
	$(CC) -c $< -o ./_build/string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

linked_list.o: ./innerStd/dataStructures/LinkedList.c
	$(CC) -c $< -o ./_build/linked_list.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

terminal.o: ./vgaBufferTerminal/terminal.c
	$(CC) -c $< -o ./_build/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

stack.o: ./innerStd/dataStructures/Stack.c
	$(CC) -c $< -o ./_build/stack.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

commands.o: ./commandHandling/commands.c
	$(CC) -c $< -o ./_build/commands.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

command_handling.o: ./commandHandling/command_handling.c
	$(CC) -c $< -o ./_build/command_handling.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

parser.o: ./innerStd/parser.c
	$(CC) -c $< -o ./_build/parser.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

pic8259.o: ./interrupts/programmableInterruptController/PIC8259.c
	$(CC) -c $< -o ./_build/pic8259.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

keyboard.o: ./interrupts/devices/keyboard.c
	$(CC) -c $< -o ./_build/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

keyboard_isr_c.o: ./interrupts/interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard_isr.c
	$(CC) -c $< -o ./_build/keyboard_isr_c.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

exception_asm.o: ./interrupts/interruptServiceRoutines/exceptions/general_exception_handler.s
	$(AS) $< -o ./_build/exception_asm.o

exception_c.o: ./interrupts/interruptServiceRoutines/exceptions/general_exception_handler.c
	$(CC) -c $< -o ./_build/exception_c.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o: boot.s
	$(AS) boot.s -o ./_build/boot.o

clean:
	rm -rf ./_build/*.o