# Define the cross-compiler tool paths
CROSS_BIN := $(HOME)/cross_compiler/install/bin
CC := $(CROSS_BIN)/i686-elf-gcc
AS := $(CROSS_BIN)/i686-elf-as

all: cassandra

cassandra: kernel.o boot.o exception_asm.o exception_c.o
	$(CC) -T ./_build/linker.ld -o cassandra -ffreestanding -O2 -nostdlib \
	    ./_build/boot.o \
	    ./_build/exception_asm.o \
	    ./_build/exception_c.o \
	    ./_build/kernel.o \
	    -lgcc

kernel.o: kernel.c
	$(CC) -c kernel.c -o ./_build/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

exception_asm.o: ./interrupts/interruptServiceRoutines/exceptions/general_exception_handler.s
	$(AS) $< -o ./_build/exception_asm.o

exception_c.o: ./interrupts/interruptServiceRoutines/exceptions/general_exception_handler.c
	$(CC) -c $< -o ./_build/exception_c.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o: boot.s
	$(AS) boot.s -o ./_build/boot.o

clean:
	rm -rf ./_build/*.o