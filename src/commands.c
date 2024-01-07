#include <stdio.h>
#include "commands.h"
#include "output.h"

bool interpret_command(command_id verb) {
    switch (verb) {
        case CMD_QUIT_I:
            puts("It is okay to quit.");
            return false;
        break;
        default:
            puts("I don't know how to do that.");
            return true;
    }

    return true;
}