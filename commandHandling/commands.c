#include "commands.h"
#include "../innerStd/string.h"
#include "../vgaBufferTerminal/terminal.h"
#include "../innerStd/parser.h"

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
    (void)args;
    return get_dynamic_string_from_char_seq("\nNOT IMPLEMENTED YET");
}