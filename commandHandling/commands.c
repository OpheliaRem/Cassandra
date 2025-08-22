#include "commands.h"
#include "../innerStd/string.h"
#include "../vgaBufferTerminal/terminal.h"
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
    terminal_write("\nUnknown command\n");
}

void command_echo(const char* args) {
    terminal_writeln("");
    terminal_writeln(args);
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
        terminal_write("\nNo such command found: ");
        terminal_writeln(args);
        return;
    }

    terminal_writeln("");
    terminal_writeln(description);
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
        terminal_write("\nNo function found: ");
        terminal_writeln(command_to_measure_name);
        free(command_to_measure_args);
        free(command_to_measure_name);
        return;
    }

    free(command_to_measure_name);

    current_ctx.args = command_to_measure_args;

    uint64_t millis = pit_measure_time(command_wrapper);

    free(command_to_measure_args);

    char* millis_str = int_to_string(millis);

    if (!millis_str) {
        terminal_writeln("\nInternal error occured. Please try again");
        return;
    }

    terminal_writeln("");
    terminal_writeln(millis_str);
}