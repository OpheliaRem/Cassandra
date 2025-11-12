#ifndef COMMANDS
#define COMMANDS

#include "../include_path.h"
#include STD

#define COMMAND_RETURN_VALUE void
#define COMMAND_ARGS (Vector args)
#define DEFINE_COMMAND(name) COMMAND_RETURN_VALUE name COMMAND_ARGS
typedef COMMAND_RETURN_VALUE(*CommandType)COMMAND_ARGS;

DEFINE_COMMAND(command_mistake);
DEFINE_COMMAND(command_echo);
DEFINE_COMMAND(command_clear);
DEFINE_COMMAND(command_newline);
DEFINE_COMMAND(command_help);
DEFINE_COMMAND(command_sleep);
DEFINE_COMMAND(command_measure_command_millis);
DEFINE_COMMAND(command_reboot);

#endif