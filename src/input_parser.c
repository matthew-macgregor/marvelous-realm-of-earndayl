#include <stdio.h>
#include <string.h>
#include "commands.h"

command_id parse_raw_input(char *input) {
    char *verb = strtok(input, " \n");
    // char *noun = strtok(NULL, " \n");

    if (verb != NULL)
    {
        if (strcmp(verb, CMD_QUIT_S) == 0)
        {
            return CMD_QUIT_I;
        }
    }

    return CMD_UNKNOWN_I;
}
