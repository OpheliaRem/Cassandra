#ifndef COMMAND_HANDLING
#define COMMAND_HANDLING

#include "../innerStd/dataStructures/HashMap.h"

extern HashMap map_command_names_and_commands;

void init_command_handling(void);
void handle_command(const char* command);

#endif