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

char* command_mistake(const char* args) {
    (void)args;
    return get_dynamic_string_from_char_seq("\nUnknown command");
}

char* command_echo(const char* args) {
    if (*args == ' ') {
        args++;
    }
    terminal_writeln("");
    terminal_writeln(args);
    return get_dynamic_string_from_char_seq("");
}

char* command_clear(const char* args) {
    (void)args;
    terminal_clear();
    return get_dynamic_string_from_char_seq("");
}

char* command_newline(const char* args) {
    (void)args;
    return get_dynamic_string_from_char_seq("");
}

char* command_help(const char* args) {
    if (args[0] == ' ') {
        ++args;
    }

    const char* description = hash_map_get(&map_command_names_and_descriptions, args);

    if (!description) {
        return get_dynamic_string_from_char_seq("\nNo such command found.");
    }

    char* result = strcat_dynamic("\n", description);

    return result;
}

//Doesn't work. TODO: fix (eternal sleep)
char* command_sleep(const char* args) {
    if (args[0] == ' ') {
        ++args;
    }

    uint64_t millis = string_to_int(args);

    pit_sleep(millis);

    return get_dynamic_string_from_char_seq("");
}

char* command_measure_command_millis(const char* args) {
    if (args[0] == ' ') {
        ++args;
    }

    size_t name_len;
    size_t command_args_len;
    char* command_to_measure_name = parse(args, 0, ' ', &name_len);
    char* command_to_measure_args = parse(args, name_len, '\0', &command_args_len);

    current_ctx.func = hash_map_get(&map_command_names_and_commands, command_to_measure_name);

    if (!current_ctx.func) {
        char* result = strcat_dynamic("\nNo function found: ", command_to_measure_name);
        free(command_to_measure_args);
        free(command_to_measure_name);
        return result;
    }

    free(command_to_measure_name);

    current_ctx.args = command_to_measure_args;

    uint64_t millis = pit_measure_time(command_wrapper);

    free(command_to_measure_args);

    char* millis_str = int_to_string(millis);

    if (!millis_str) {
        return get_dynamic_string_from_char_seq("\nInternal error, please try again");
    }

    return millis_str;
}