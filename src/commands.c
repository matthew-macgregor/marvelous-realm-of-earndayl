#include <assert.h>
#include <stdio.h>
#include "commands.h"
#include "colors.h"
#include "constants.h"
#include "entry.h"
#include "hero.h"
#include "input_parser.h"
#include "directions.h"
#include "connectors.h"
#include "inventory.h"

static bool cmd_drop(void);
static bool cmd_empty(void);
static bool cmd_get(void);
static bool cmd_inventory(void);
static bool cmd_look(void);
static bool cmd_look_in(void);
static bool cmd_move(void);
static bool cmd_quit(void);
static bool cmd_unknown(const char *input);

#define END_OF_COMMANDS { NULL, NULL }
static const Command commands[] = {
    { "quit",           cmd_quit        },
    { "exit",           cmd_quit        },
    { "q",              cmd_quit        },
    { "bye",            cmd_quit        },
    { "empty A",        cmd_empty       },
    { "look in A",      cmd_look_in     },
    { "look at A",      NULL            },
    { "look around",    cmd_look        },
    { "look",           cmd_look        },
    { "go A",           cmd_move        },
    { "get A",          cmd_get         },
    { "inventory",      cmd_inventory   },
    { "drop A",         cmd_drop        },
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

    if (matched_cmd->function == NULL) {
        printf(CON_RED "Sorry, I can't help you '%s'.\n", input);
        return true;
    }

    return matched_cmd->function != NULL ? (*matched_cmd->function)() : false;
}

static bool cmd_unknown(const char *input) {
    printf(CON_RED "Sorry, I don't know how to '%s'." CON_RESET, input);
    return true;
}

static bool cmd_drop(void) {
    Entry *here = hero_get_entry();
    Entry *inventory = inv_get_inventory_entry();
    const char *phrase = get_captured_phrase('A');
    assert(here != NULL && inventory != NULL && *phrase != '\0');

    Entry *obj = entry_search_by_trait_and_entry_id(phrase, inventory->id);
    if (obj != NULL) {
        // printf("You're in luck, '%s' is here.\n", phrase);
        if (inv_remove_object_from_inventory(obj, here)) {
            printf("You drop %s.", obj->short_description);
        } else {
            printf("You fail to drop the %s.\n", obj->short_description);
        }
    } else {
        printf(CON_RED "Sorry, I don't see a '%s' to drop." CON_RESET, phrase);
    }

    return true;
}

static bool cmd_empty(void) {
    // TODO: routine here is very close to cmd_look_in. DRY?
    Entry *here = hero_get_entry();
    size_t entry_count = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    const char *phrase = get_captured_phrase('A');
    Entry *container = entry_search_by_trait_and_entry_id(phrase, here->id);
    if (container == NULL) {
        printf("You don't see a %s here.\n", phrase);
        return true;
    }

    for (size_t i = 0; i < entry_count; i++) {
        Entry obj = entries[i];
        int emptied_count = 0;
        if (obj.entry == container) {
            (&entries[i])->entry = here;
            // If the item's location is the container, it's in the container.
            printf(
                "%sYou remove %s from %s.",
                emptied_count > 0 ? "\n" : "",
                obj.short_description,
                container->short_description);
            emptied_count++;
        }
    }

    return true;
}

static bool cmd_get(void) {
    Entry *here = hero_get_entry();
    const char *phrase = get_captured_phrase('A');
    Entry *entry = entry_search_by_trait_and_entry_id(phrase, here->id);
    const int encumbrance = inv_get_encumbrance();

    if (entry != NULL) {
        if (entry->heft == IS_STATIC) {
            printf(CON_YELLOW "Do you really think you can lift that?" CON_RESET);
            return true;
        } else if (entry->heft + encumbrance > 20) { // TODO: 20 is arbitrary here
            printf("Sorry, that's too much for you to carry.");
            return true;
        }

        if (inv_add_object_to_inventory(entry)) {
            printf("You pick up %s.", entry->short_description);
        } else {
            printf("You fail to get the %s.", entry->short_description);
        }
    } else {
        printf(CON_RED "Sorry, no '%s' is here." CON_RESET, phrase);
    }

    return true;
}

static bool cmd_inventory(void) {
    Entry *inventory = inv_get_inventory_entry();
    EntryArrayPtr entries = entry_get_entries();
    size_t entry_count = entry_get_entry_count();

    printf("Inventory ");
    bool found_obj = false;
    for (size_t i = 0; i < entry_count; i++) {
        Entry obj = entries[i];
        if (obj.entry == inventory) {
            printf("\n - %s", obj.short_description);
            found_obj = true;
        }
    }

    found_obj ? printf("\n"): printf("is %s%s" CON_RESET, CON_YELLOW, "empty.\n");
    return true;
}

static bool cmd_look_objects(void) {
    Entry *here = hero_get_entry();
    size_t entry_count = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();

    bool found_obj = false;
    for (size_t i = 0; i < entry_count; i++) {
        Entry obj = entries[i];
        if (obj.entry == here) {
            printf("%sYou see %s here.",
                found_obj ? "\n" : "",
                obj.short_description);
            found_obj = true;
        }
    }

    if (found_obj == false) {
        printf("You see nothing very interesting here.");
    }
    return true;
}

static bool cmd_look(void) {
    // const Hero *hero = hero_get_hero();
    entry_id location = hero_get_entry_id();
    Entry *entry = entry_get_by_entry_id(location);
    if (entry != NULL) {
        const char *description = entry_get_short_description(entry);
        printf("You are in %s. ", description);

        const char *exits = entry_get_exits(entry);
        printf("(%s)\n", exits);

        cmd_look_objects();
    } else {
        printf("%s", "Not sure what you want to look at!");
    }

    return true;
}

static bool cmd_look_in(void) {
    Entry *here = hero_get_entry();
    size_t entry_count = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    const char *phrase = get_captured_phrase('A');
    Entry *container = entry_search_by_trait_and_entry_id(phrase, here->id);
    if (container == NULL) {
        printf("You don't see a %s here.\n", phrase);
        return true;
    }

    bool found_obj = false;
    printf("In %s, you see:", container->short_description);
    for (size_t i = 0; i < entry_count; i++) {
        Entry obj = entries[i];
        if (obj.entry == container) {
            // If the item's location is the container, it's in the container.
            printf("\n - %s", obj.short_description);
            found_obj = true;
        }
    }

    found_obj ? 
        printf("\n(Hint: try the 'empty' command on %s!)\n", container->short_description) : 
        printf("nothing.\n");
    return true;
}

static bool cmd_move(void) {
    entry_id location = hero_get_entry_id();
    Entry *entry = entry_get_by_entry_id(location);
    if (entry != NULL) {
        const char *phrase = get_captured_phrase('A');
        const Direction dir = direction_text_to_direction(phrase);
        const char *dir_text = direction_to_text(dir);
        const entry_id loc = connector_get_location_id_in_direction(entry, dir);

        if (loc == ENTRY_UNKNOWN) {
            printf("You can't go '%s' from here.", dir_text);
        } else {
            printf("You move cautiously to the %s.\n", dir_text);
            if (!hero_set_entry_id(loc)) {
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
