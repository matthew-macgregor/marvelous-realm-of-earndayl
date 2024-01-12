#include <stdio.h>
#include "commands.h"
#include "output.h"
#include "entry.h"
#include "input_parser.h"

static bool cmd_quit(void); // TODO
static bool cmd_unknown(const char *input);
static bool cmd_look(void);

#define END_OF_COMMANDS { NULL, NULL }

bool interpret_command(const char *input) {
    static const Command commands[] = {
        { "quit",           cmd_quit        },
        { "look",           cmd_look        },
        { "look around",    NULL            },
        { "look at A",      NULL            },
        END_OF_COMMANDS
    };

    const Command *matched_cmd;
    CapturedPhraseResult result = { 0, 0, false };
    for (const Command *cmd = commands; !result.matched && cmd->pattern != NULL; cmd++) {
        result = parse_pattern(input, cmd->pattern);
        if (result.matched) matched_cmd = cmd;
    }

    if (!result.matched) {
        return cmd_unknown(input);
    }

    return matched_cmd->function != NULL ? (*matched_cmd->function)() : false;
}

static bool cmd_unknown(const char *input) {
    (void)input; // silence unused parameter
    puts("Sorry, I don't know how to '%s'.", input);
    return true;
}

static bool cmd_look() {
    puts_ok("%s", "Not sure what you want to look at!");
    return true;
}

static bool cmd_quit(void) {
    puts_ok("%s", "Okay, bye!");
    return false;
}