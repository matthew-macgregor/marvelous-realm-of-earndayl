#include <stdio.h>
#include "commands.h"
#include "output.h"
#include "rooms.h"

#ifndef SILENT
static unsigned player_location = 0;
#endif

bool interpret_command(command_id verb) {
    switch (verb) {
        case CMD_QUIT_I:
            puts("It is okay to quit.");
            return false;
        break;
        case CMD_LOOK_I:
            cmd_look("around");
        break;
        default:
            puts("I don't know how to do that.");
            return true;
    }

    return true;
}

void cmd_look(const char *phrase)
{
   if (phrase != NULL && strcmp(phrase, "around") == 0)
   {
      printf_s("You are in %s.\n", rooms[player_location].description);
   }
   else
   {
      puts("I don't understand what you want to see.");
   }
}