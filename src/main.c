/* Game Main Entrypoint */
#include <stdio.h>
#include <stdbool.h>

#include "colors.h"
#include "commands.h"
#include "constants.h"
#include "input_parser.h"
#include "output.h"

#define STRING_UTILS_IMPLEMENTATION
#include "string_utils.h"

static bool get_input(char *input) {
   printf("> ");
   return fgets(input, INPUT_MAX_LENGTH, stdin) != NULL;
}

int main() {
    static char input[100];
    puts_warn("%s", "------------------------------------------");
    puts_ok(  "%s", "Welcome to the Marvelous Realm of Eärndayl");
    puts_warn("%s", "------------------------------------------");
    bool result = true;
    do {
        get_input(input);
        STRIP_NEWLINE(input);
        if (is_empty_or_whitespace(input)) continue;
        result = interpret_command(input);
        printf("\n");
    } while(result);
    return 0;
}
