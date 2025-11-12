#ifndef COMMAND_HANDLING
#define COMMAND_HANDLING

#include "../include_path.h"
#include STD

extern HashMap map_command_names_and_commands;
extern HashMap map_command_names_and_descriptions;

void init_command_handling(void);
void handle_command(const char* command);

#endif