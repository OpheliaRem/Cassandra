CROSS_BIN := $(HOME)/opt/cross/bin
CC := $(CROSS_BIN)/i686-elf-gcc
AS := $(CROSS_BIN)/i686-elf-as

all: cassandra

run: cassandra
	qemu-system-i386 -kernel cassandra

cassandra: kernel.o boot.o exception_asm.o exception_c.o terminal.o string.o allocator.o convert.o idt_initialization.o gdt_init.o stack.o commands.o command_handling.o pic8259.o keyboard.o linked_list.o hash_map.o interrupt_descriptor_table_entry.o pit.o tss.o syscall.o process.o user_memory.o isr80stub.o hello_bin.o vector.o io.o std.o
	$(CC) -T ./linker.ld -o cassandra -ffreestanding -O2 -nostdlib \
	    ./_build/boot.o \
	    ./_build/exception_asm.o \
	    ./_build/exception_c.o \
		./_build/terminal.o \
		./_build/stack.o \
		./_build/commands.o \
		./_build/command_handling.o \
		./_build/string.o \
		./_build/linked_list.o \
		./_build/hash_map.o \
		./_build/vector.o \
		./_build/allocator.o \
		./_build/io.o \
		./_build/convert.o \
		./_build/interrupt_descriptor_table_entry.o \
		./_build/gdt_init.o \
		./_build/idt_initialization.o \
		./_build/pic8259.o \
		./_build/keyboard.o \
		./_build/pit.o \
		./_build/tss.o \
		./_build/syscall.o \
		./_build/process.o \
		./_build/user_memory.o \
		./_build/isr80stub.o \
		./_build/std.o \
		./hello_bin.o \
	    ./_build/kernel.o \
	    -lgcc

kernel.o: kernel.c
	$(CC) -c kernel.c -o ./_build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

std.o: std.c
	$(CC) -c $< -o ./_build/std.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

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

vector.o: ./innerStd/dataStructures/Vector.c
	$(CC) -c $< -o ./_build/vector.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

hash_map.o: ./innerStd/dataStructures/HashMap.c
	$(CC) -c $< -o ./_build/hash_map.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

terminal.o: ./vgaBufferTerminal/terminal.c
	$(CC) -c $< -o ./_build/terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

stack.o: ./innerStd/dataStructures/Stack.c
	$(CC) -c $< -o ./_build/stack.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

io.o: ./innerStd/io.c
	$(CC) -c $< -o ./_build/io.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

commands.o: ./commandHandling/commands.c
	$(CC) -c $< -o ./_build/commands.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

command_handling.o: ./commandHandling/command_handling.c
	$(CC) -c $< -o ./_build/command_handling.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

pic8259.o: ./interrupts/programmableInterruptController/PIC8259.c
	$(CC) -c $< -o ./_build/pic8259.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

keyboard.o: ./interrupts/interruptServiceRoutines/hardwareInterrupts/keyboard/keyboard.c
	$(CC) -c $< -o ./_build/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

pit.o: ./interrupts/interruptServiceRoutines/hardwareInterrupts/programmableIntervalTimer/pit.c
	$(CC) -c $< -o ./_build/pit.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

interrupt_descriptor_table_entry.o: ./interrupts/interruptDescriptorTable/InterruptDescriptorTableEntry.c
	$(CC) -c $< -o ./_build/interrupt_descriptor_table_entry.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

syscall.o: ./userSpace/syscall/syscall.c
	$(CC) -c $< -o ./_build/syscall.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

process.o: ./userSpace/process.c
	$(CC) -c $< -o ./_build/process.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

user_memory.o: ./userSpace/user_memory.c
	$(CC) -c $< -o ./_build/user_memory.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

isr80stub.o: ./interrupts/interruptServiceRoutines/softwareInterrupts/isr80stub.s
	$(AS) $< -o ./_build/isr80stub.o

tss.o: ./gdt/TaskStateSegment.c
	$(CC) -c $< -o ./_build/tss.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

exception_asm.o: ./interrupts/interruptServiceRoutines/exceptions/general_exception_handler.s
	$(AS) $< -o ./_build/exception_asm.o

exception_c.o: ./interrupts/interruptServiceRoutines/exceptions/general_exception_handler.c
	$(CC) -c $< -o ./_build/exception_c.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o: boot.s
	$(AS) boot.s -o ./_build/boot.o

hello_bin.o: ./_build/hello.bin
	/usr/local/opt/binutils/bin/gobjcopy -I binary -O elf32-i386 $< hello_bin.o

hello.bin: ./_build/hello.o
	/usr/local/opt/binutils/bin/gobjcopy -O binary $< ./_build/hello.bin

hello.o: hello.asm
	nasm -f elf32 $< -o ./_build/hello.o

clean:
	rm -rf ./_build/*.o