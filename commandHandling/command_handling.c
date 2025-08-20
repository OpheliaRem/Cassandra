#include "command_handling.h"
#include "commands.h"
#include "../innerStd/allocator.h"
#include "../innerStd/string.h"
#include "../innerStd/parser.h"
#include "../vgaBufferTerminal/terminal.h"

static char* (*determine_command(const char* name))(const char*) {
    if (are_equal_strings("echo", name)) {
        return command_echo;
    }

    if (are_equal_strings("clear", name)) {
        return command_clear;
    }

    if (are_equal_strings("", name)) {
        return command_newline;
    }

    return command_mistake;
}

void handle_command(const char* command) {
    size_t name_len = 0;
    char* name = parse(command, 0, ' ', &name_len);

    if (!name) {
        return;
    }

    char*(*action)(const char*) = determine_command(name);

    free(name);

    size_t args_len = 0;
    char* args = parse(command, name_len, '\0', &args_len);

    if (!args) {
        return;
    }

    char* message = action(args);

    free(args);

    if (!message) {
        return;
    }

    terminal_write(message);

    free(message);
}
