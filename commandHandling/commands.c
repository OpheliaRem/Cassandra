#include "commands.h"
#include "../innerStd/string.h"
#include "../vgaBufferTerminal/terminal.h"
#include "../innerStd/io.h"
#include "../innerStd/parser.h"
#include "../innerStd/allocator.h"
#include "../innerStd/convert.h"
#include "command_handling.h"
#include "../interrupts/interruptServiceRoutines/hardwareInterrupts/programmableIntervalTimer/pit.h"

struct CommandContext {
    char* (*func)(const char* args);
    const char* args;
    char* result;
};

static struct CommandContext current_ctx;

static void command_wrapper(void) {
    current_ctx.result = current_ctx.func(current_ctx.args);
    free(current_ctx.result);
}

void command_mistake(const char* args) {
    (void)args;
    print("\nUnknown command\n");
}

void command_echo(const char* args) {
    printf("\n%s\n", args);
}

void command_clear(const char* args) {
    (void)args;
    terminal_clear();
}

void command_newline(const char* args) {
    (void)args;
}

void command_help(const char* args) {
    const char* description = hash_map_get(&map_command_names_and_descriptions, args);

    if (!description) {
        printf("\nNo such command found: %s", args);
        return;
    }

    printf("\n%s\n", description);
}

void command_sleep(const char* args) {
    pit_sleep(string_to_int(args));
}

void command_measure_command_millis(const char* args) {
    size_t name_len;
    size_t command_args_len;
    char* command_to_measure_name = parse(args, 0, ' ', &name_len);
    char* command_to_measure_args = parse(args, name_len, '\0', &command_args_len);

    current_ctx.func = hash_map_get(&map_command_names_and_commands, command_to_measure_name);

    if (!current_ctx.func) {
        printf("\nNo function found: %s\n", command_to_measure_name);
        free(command_to_measure_args);
        free(command_to_measure_name);
        return;
    }

    free(command_to_measure_name);

    current_ctx.args = command_to_measure_args;

    uint64_t millis = pit_measure_time(command_wrapper);

    free(command_to_measure_args);

    printf("\n%d\n", millis);
}

static inline void outb(uint8_t value, uint16_t port) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void command_reboot(const char* args) {
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