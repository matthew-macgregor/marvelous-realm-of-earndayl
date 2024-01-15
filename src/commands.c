#include <stdio.h>
#include "commands.h"
#include "colors.h"
#include "entry.h"
#include "hero.h"
#include "input_parser.h"
#include "directions.h"
#include "connectors.h"
#include "objects.h"
#include "inventory.h"

static bool cmd_quit(void); // TODO
static bool cmd_unknown(const char *input);
static bool cmd_look(void);
static bool cmd_move(void);
static bool cmd_get(void);

#define END_OF_COMMANDS { NULL, NULL }
static const Command commands[] = {
    { "quit",           cmd_quit        },
    { "exit",           cmd_quit        },
    { "q",              cmd_quit        },
    { "bye",            cmd_quit        },
    { "look",           cmd_look        },
    { "look around",    cmd_look        },
    { "look at A",      NULL            },
    { "go A",           cmd_move        },
    { "get A",          cmd_get         },
    END_OF_COMMANDS
};

bool interpret_command(const char *input) {
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
    printf(CON_RED "Sorry, I don't know how to '%s'." CON_RESET, input);
    return true;
}

static bool cmd_get(void) {
    Entry *here = hero_get_entry();
    Entry *inventory = inv_get_inventory_entry();
    (void)inventory;
    const char *phrase = get_captured_phrase('A');
    Object *obj = obj_search_by_trait_and_location_id(phrase, here->id);
    if (obj != NULL) {
        // printf("You're in luck, '%s' is here.\n", phrase);
        if (inv_add_object_to_inventory(obj)) {
            printf("You pick up %s.", obj->description);
        } else {
            printf("You fail to get the %s.\n", obj->description);
        }
    } else {
        printf(CON_RED "Sorry, no '%s' is here." CON_RESET, phrase);
    }

    return true;
}

static bool cmd_look_objects(void) {
    Entry *entry = hero_get_entry();
    size_t obj_count = obj_get_object_count();
    ObjectArrayPtr objects = obj_get_objects();

    printf("You see: ");
    bool found_obj = false;
    for (size_t i = 0; i < obj_count; i++) {
        Object obj = objects[i];
        if (obj.location == entry) {
            printf("\n - %s", obj.description);
            found_obj = true;
        }
    }

    found_obj ? printf("\n") : printf("nothing.\n");
    return true;
}

static bool cmd_look(void) {
    // const Hero *hero = hero_get_hero();
    location_id location = hero_get_location_id();
    Entry *entry = entry_get_by_location_id(location);
    if (entry != NULL) {
        const char *description = entry_get_short_description(entry);
        printf("You are in %s.\n", description);

        cmd_look_objects();

        const char *exits = entry_get_exits(entry);
        printf("Exits: %s\n", exits);
    } else {
        printf("%s", "Not sure what you want to look at!");
    }

    return true;
}

static bool cmd_move(void) {
    location_id location = hero_get_location_id();
    Entry *entry = entry_get_by_location_id(location);
    if (entry != NULL) {
        const char *phrase = get_captured_phrase('A');
        const Direction dir = direction_text_to_direction(phrase);
        const char *dir_text = direction_to_text(dir);
        const location_id loc = connector_get_location_id_in_direction(entry, dir);

        if (loc == LOCATION_UNKNOWN) {
            printf("You can't go '%s' from here.", dir_text);
        } else {
            printf("You move cautiously to the %s.\n", dir_text);
            if (!hero_set_location_id(loc)) {
                printf("Yikes, something went wrong setting location.\n");
            } else {
                cmd_look();
            }
        }
    } else {
        printf(CON_RED "%s\n" CON_RESET, "You may have gotten lost.");
    }

    return true;
}

static bool cmd_quit(void) {
    printf("%s\n", "Okay, bye!");
    return false;
}
