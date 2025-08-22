#include "command_handling.h"
#include "commands.h"
#include "../innerStd/allocator.h"
#include "../innerStd/string.h"
#include "../innerStd/parser.h"
#include "../vgaBufferTerminal/terminal.h"
#include <stdbool.h>

HashMap map_command_names_and_commands;

static char* (*determine_command(const char* name))(const char*) {
    char*(*command)(const char*) = hash_map_get(&map_command_names_and_commands, name);

    if (command) {
        return command;
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

static size_t hash(const void* ptr) {
    char* str = (char*)ptr;

    size_t sum = 0;
    while (*str) {
        sum += *str;
        str++;
    }

    return sum;
}

static bool are_equal(const void* a, const void* b) {
    return are_equal_strings((char*)a, (char*)b);
}

void init_command_handling(void) {
    hash_map_init(&map_command_names_and_commands, hash, are_equal);

    hash_map_add(&map_command_names_and_commands, "echo", command_echo);
    hash_map_add(&map_command_names_and_commands, "clear", command_clear);
    hash_map_add(&map_command_names_and_commands, "", command_newline);
    hash_map_add(&map_command_names_and_commands, "help", command_help);
}
