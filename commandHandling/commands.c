#include "commands.h"
#include "../vgaBufferTerminal/terminal.h"
#include "command_handling.h"
#include "../interrupts/interruptServiceRoutines/hardwareInterrupts/programmableIntervalTimer/pit.h"
#include "../include_path.h"
#include STD

/* struct CommandContext {
    char* (*func)(const char* args);
    const char* args;
    char* result;
};

static struct CommandContext current_ctx;

static void command_wrapper(void) {
    current_ctx.result = current_ctx.func(current_ctx.args);
    free(current_ctx.result);
} */

DEFINE_COMMAND(command_mistake) {
    printf("\nUnknown command: %s\n", vector_immutable_get(&args, 0));
}

DEFINE_COMMAND(command_echo) {
    println("");
    for (size_t i = 1; i < args.size; ++i) {
        printf("%s", vector_immutable_get(&args, i));
        if (i != args.size - 1) {
            print(" ");
        }
    }
    println("");
}

DEFINE_COMMAND(command_clear) {
    (void)args;
    terminal_clear();
}

DEFINE_COMMAND(command_newline) {
    (void)args;
}

DEFINE_COMMAND(command_help) {
    const char* command_name = vector_immutable_get(&args, 1);

    if (!command_name) {
        printf("\nhelp: couldn't find a command name\n");
        return;
    }

    const char* description = hash_map_get(&map_command_names_and_descriptions, command_name);

    if (!description) {
        printf("\nhelp: No such command found: %s", command_name);
        return;
    }

    printf("\n%s\n", description);
}

DEFINE_COMMAND(command_sleep) {
    const char* num = vector_immutable_get(&args, 1);

    if (!num) {
        printf("\nsleep: couldn't get a number to use\n");
        return;
    }

    pit_sleep(atoi(num));
}

DEFINE_COMMAND(command_measure_command_millis) {
    (void)args;
    printf("\nSorry, this command was defined in the standard, but it is still in progress\n");
}

static inline void outb(uint8_t value, uint16_t port) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

DEFINE_COMMAND(command_reboot) {
    (void)args;

    #define KEYBOARD_STATUS_PORT 0x64
    #define KEYBOARD_CMD_PULSE   0xFE
    #define KEYBOARD_STATUS_INPUT_FULL 0x02

    uint8_t status;

    asm volatile("cli");

    do {
        status = inb(KEYBOARD_STATUS_PORT);
    } while (status & KEYBOARD_STATUS_INPUT_FULL);

    outb(KEYBOARD_CMD_PULSE, KEYBOARD_STATUS_PORT);

    asm volatile ("hlt");
}