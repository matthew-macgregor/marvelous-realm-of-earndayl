/* Game Main Entrypoint */
#include <stdio.h>
#include <stdbool.h>

#include "colors.h"
#include "commands.h"
#include "constants.h"
#include "input_parser.h"

#define STRING_UTILS_IMPLEMENTATION
#include "string_utils.h"
#include "constants.h"

static bool get_input(char *input) {
   printf("> ");
   return fgets(input, INPUT_MAX_LENGTH, stdin) != NULL;
}

int main(void) {
    static char input[INPUT_MAX_LENGTH];
    printf(CON_BLUE   "%s\n" CON_RESET, "------------------------------------------");
    printf(CON_YELLOW "%s\n" CON_RESET, "Welcome to the Marvelous Realm of Eärndayl");
    printf(CON_BLUE   "%s\n" CON_RESET, "------------------------------------------");
    bool result = true;
    do {
        if (get_input(input)) {
            s_strip_newline(input, INPUT_MAX_LENGTH);
            if (is_empty_or_whitespace(input)) continue;
            result = interpret_command(input);
            printf("\n");
        } else {
            break;
        }
    } while(result);
    return 0;
}
