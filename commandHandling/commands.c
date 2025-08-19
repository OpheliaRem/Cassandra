#include "commands.h"
#include "../innerStd/string.h"
#include "../vgaBufferTerminal/terminal.h"

char* command_mistake(const char* args) {
    (void)args;
    return get_dynamic_string_from_char_seq("Unknown command");
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
    terminal_print_new_prompt();
    return get_dynamic_string_from_char_seq("");
}