#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

typedef struct Command {
   const char *pattern;
   bool (*function)(void);
} Command;

/**
 * Returns true to continue the gameloop, false to exit. 
 */
bool interpret_command(const char *input);

#endif