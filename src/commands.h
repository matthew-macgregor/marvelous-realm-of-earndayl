#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

typedef int command_id;

#define CMD_QUIT_S  "quit"
#define CMD_QUIT_I  0
#define CMD_LOOK_S  "look"
#define CMD_LOOK_I  100

#define CMD_UNKNOWN_I   -1

/**
 * Returns true to continue the gameloop, false to exit. 
 */
bool interpret_command(command_id verb);


void cmd_look(const char *noun);

#endif