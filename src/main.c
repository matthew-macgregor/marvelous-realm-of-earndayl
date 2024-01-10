/* Game Main Entrypoint */
#include <stdio.h>
#include <stdbool.h>

#include "commands.h"
#include "input_parser.h"
#include "output.h"

static char input[100] = "";

static bool get_input(void) {
   printf("\n> ");
   return fgets(input, sizeof input, stdin) != NULL;
}

int main() {
    puts("Welcome to the Marvelous Realm of Eärndayl");
    COMMAND_ID cmd;
    do {
        get_input();
        cmd = parse_raw_input(input);
    } while(interpret_command(cmd));
    return 0;
}
