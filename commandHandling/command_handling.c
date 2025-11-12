#include <stdbool.h>
#include "command_handling.h"
#include "commands.h"
#include "../include_path.h"
#include STD

HashMap map_command_names_and_commands;
HashMap map_command_names_and_descriptions;

static CommandType determine_command(const char* name) {
    CommandType command = hash_map_get(&map_command_names_and_commands, name);

    if (command) {
        return command;
    }

    return command_mistake;
}

void handle_command(const char* input) {
    char* command = string_copy(input);
    string_mutate_trim(&command, ' ');
    Vector args = string_split(command, ' ');
    free(command);

    const char* name = vector_immutable_get(&args, 0);
    if (!name) {
        return;
    }

    CommandType action = determine_command(name);

    action(args);

    vector_foreach(&args, free);
    vector_free(&args);
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
    return string_compare((const char*)a, (const char*)b);
}

void init_command_handling(void) {
    hash_map_init(&map_command_names_and_commands, hash, are_equal);

    hash_map_add(&map_command_names_and_commands, "echo", command_echo);
    hash_map_add(&map_command_names_and_commands, "clear", command_clear);
    hash_map_add(&map_command_names_and_commands, "", command_newline);
    hash_map_add(&map_command_names_and_commands, "help", command_help);
    hash_map_add(&map_command_names_and_commands, "sleep", command_sleep);
    hash_map_add(&map_command_names_and_commands, "measure-command-millis", command_measure_command_millis);
    hash_map_add(&map_command_names_and_commands, "reboot", command_reboot);

    //=====

    hash_map_init(&map_command_names_and_descriptions, hash, are_equal);

    hash_map_add(&map_command_names_and_descriptions, "echo", "format: <echo [text]>\nPrints to the VGA buffer the string following the command name skipping exactly one spacebar");
    hash_map_add(&map_command_names_and_descriptions, "clear", "format: <clear>\nClears the current terminal screen");
    hash_map_add(&map_command_names_and_descriptions, "help", "format: <help [text]>\nHelps you to find information about something, for example about a command, just like now!");
    hash_map_add(&map_command_names_and_descriptions, "sleep", "format: <sleep {milliseconds}>\nHalts the CPU for the amount of time provided in ms. The interrupts are still going to be handled.");
    hash_map_add(&map_command_names_and_descriptions, "measure-command-millis", "format: <measure-command-millis {command}>\nExecutes the provided command and measures in ms the time it has taken");
}
