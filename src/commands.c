#include <stdio.h>
#include "commands.h"
#include "output.h"
#include "rooms.h"
#include "input_parser.h"

#ifndef SILENT
static unsigned player_location = 0;
#endif

static bool cmd_quit(void); // TODO
static bool cmd_unknown(const char *input);

bool interpret_command(const char *input) {
    static const Command commands[] = {
        { "quit",           cmd_quit    },
        { "look",           NULL        },
        { "look around",    NULL        },
        { "look at A",      NULL        },
        { NULL, NULL }
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
    printf(CON_YELLOW "Sorry, I don't know how to '%s'." CON_RESET, input);
    return true;
}

static bool cmd_quit(void) {
    puts_ok("%s", "Okay, bye!");
    return false;
}

void cmd_look(const char *phrase)
{
   if (phrase != NULL && strcmp(phrase, "around") == 0)
   {
      puts("You are in %s.\n", rooms[player_location].description);
   }
   else
   {
      puts("%s", "I don't understand what you want to see.");
   }
}