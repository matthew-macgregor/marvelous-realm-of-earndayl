#include <stdio.h>
#include "commands.h"
#include "colors.h"
#include "entry.h"
#include "hero.h"
#include "input_parser.h"

static bool cmd_quit(void); // TODO
static bool cmd_unknown(const char *input);
static bool cmd_look(void);

#define END_OF_COMMANDS { NULL, NULL }

bool interpret_command(const char *input) {
    static const Command commands[] = {
        { "quit",           cmd_quit        },
        { "q",              cmd_quit        },
        { "look",           cmd_look        },
        { "look around",    cmd_look        },
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
    printf(CON_RED "Sorry, I don't know how to '%s'." CON_RESET, input);
    return true;
}

static bool cmd_look(void) {
    // const Hero *hero = hero_get_hero();
    location_id location = hero_get_location();
    Entry *entry = entry_get_by_location(location);
    if (entry != NULL) {
        const char *description = entry_get_description(entry);
        printf("You are in %s.\n", description);
        const char *exits = entry_get_exits(entry);
        printf(" - Exits: %s\n", exits);
    } else {
        printf("%s", "Not sure what you want to look at!");
    }

    return true;
}

static bool cmd_quit(void) {
    printf("%s\n", "Okay, bye!");
    return false;
}
