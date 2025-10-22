#include <assert.h>
#include <stdio.h>
#include "commands.h"
#include "colors.h"
#include "constants.h"
#include "debug.h"
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
static bool cmd_look_at(void);
static bool cmd_look_in(void);
static bool cmd_move(void);
static bool cmd_ready_weapon(void);
static bool cmd_quit(void);
static bool cmd_unknown(const char *input);

#ifdef DEBUG
static bool cmd_debug_value(void);
static bool cmd_debug_copy_to_empty(void);
#endif

#define END_OF_COMMANDS { NULL, NULL }
static const Command commands[] = {
    { "quit",           cmd_quit                     },
    { "exit",           cmd_quit                     },
    { "q",              cmd_quit                     },
    { "bye",            cmd_quit                     },
    { "empty A",        cmd_empty                    },
    { "look in A",      cmd_look_in                  },
    { "look at A",      cmd_look_at                  },
    { "look A",         cmd_look_at                  },
    { "look around",    cmd_look                     },
    { "look",           cmd_look                     },
    { "go A",           cmd_move                     },
    { "get A",          cmd_get                      },
    { "i",              cmd_inventory                },
    { "inv",            cmd_inventory                },
    { "inventory",      cmd_inventory                },
    { "drop A",         cmd_drop                     },
    { "arm A",          cmd_ready_weapon             },
#ifdef DEBUG
    { "debug value A",  cmd_debug_value              },
    { "debug copy-to-empty", cmd_debug_copy_to_empty },
#endif
    END_OF_COMMANDS
};

bool interpret_command(const char *input) {
    const Command *matched_cmd;
    CapturedPhraseResult result = { 0, 0, false };
    for (const Command *cmd = commands; !result.matched && cmd->pattern != NULL; cmd++) {
        result = parse_pattern(input, cmd->pattern);
        if (result.matched) {
            matched_cmd = cmd;
            break;
        }
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
    const char *phrase = parse_get_captured_phrase('A');
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
    const char *phrase = parse_get_captured_phrase('A');
    Entry *container = entry_search_by_trait_and_entry_id(phrase, here->id);
    if (container == NULL) {
        printf("You don't see a %s here.\n", phrase);
        return true;
    }

	if (!entry_setting_is_enabled(container, ESET_ALLOW_INTROSPECT)) {
		printf("How do you think you'll empty that %s exactly?\n", phrase);
		return true;
	}

	for (size_t i = 0; i < entry_count; i++) {
        Entry obj = entries[i];
        int emptied_count = 0;
        if (obj.location == container) {
            (&entries[i])->location = here;
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
    const char *phrase = parse_get_captured_phrase('A');
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
    Hero *hero = hero_get_hero();
    Entry *inventory = inv_get_inventory_entry();
    EntryArrayPtr entries = entry_get_entries();
    size_t entry_count = entry_get_entry_count();

    printf("Inventory ");
    bool found_obj = false;
    for (size_t i = 0; i < entry_count; i++) {
        Entry *obj = &entries[i];
        if (obj->location == inventory) {
            char *armed = obj == hero->armed_weapon ? "(ARMED)" : "";
            printf("\n - %s %s", obj->short_description, armed);
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
        if (obj.location == here) {
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

static bool cmd_look_at(void) {
    Entry *here = hero_get_entry();
    const char *phrase = parse_get_captured_phrase('A');
    Entry *entry = entry_search_by_trait_and_entry_id(phrase, here->id);
    if (entry != NULL && (entry->location == here || entry->location == EP_INVENTORY)) {
        printf("%s", entry->short_description);
    } else {
        printf("You don't see a '%s'.", phrase);
    }

    return true;
}

static bool cmd_look_in(void) {
    Entry *here = hero_get_entry();
    size_t entry_count = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    const char *phrase = parse_get_captured_phrase('A');
    Entry *container = entry_search_by_trait_and_entry_id(phrase, here->id);
    if (container == NULL) {
        printf("You don't see a %s here.\n", phrase);
        return true;
    }

	if (!entry_setting_is_enabled(container, ESET_ALLOW_INTROSPECT)) {
		printf("You really can't look inside that %s.\n", phrase);
		return true;
	}

    bool found_obj = false;
    printf("In %s, you see:", container->short_description);
    for (size_t i = 0; i < entry_count; i++) {
        Entry obj = entries[i];
        if (obj.location == container) {
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
    Entry *entry = hero_get_entry();
    if (entry != NULL) {
        const char *phrase = parse_get_captured_phrase('A');
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

static bool cmd_ready_weapon(void) {
    const char *phrase = parse_get_captured_phrase('A');
    Entry *entry = entry_search_by_trait(phrase);
    if (entry == NULL || entry->location != EP_INVENTORY) {
        printf(CON_RED "Could not find %s to ready." CON_RESET, phrase);
    } else {
        if (hero_arm_weapon(entry)) {
            printf(CON_GREEN "You have %s at the ready." CON_RESET, phrase);
        } else {
            printf(CON_RED "You can't fight with something like %s." CON_RESET, phrase);
        }
    }

    return true;
}

static bool cmd_quit(void) {
    printf(CON_YELLOW "%s\n" CON_RESET, "Okay, bye!");
    return false;
}

#ifdef DEBUG
#include "dice.h"

static bool cmd_debug_value(void) {
    const char *phrase = parse_get_captured_phrase('A');
    Entry *entry = entry_search_by_trait(phrase);
    if (entry == NULL) {
        printf("Debug: can't find '%s'", phrase);
        return true;
    }

    if (entry_is_value_nil(entry)) {
        printf("Debug: no value for '%s'", phrase);
        return true;
    }

    Coins value = entry->value;

    DEBUG_PRINTF(CON_CYAN "Debug::Gold: %d\n" CON_RESET, value.gold);
    DEBUG_PRINTF(CON_CYAN "Debug::Siver: %d\n" CON_RESET, value.silver);
    DEBUG_PRINTF(CON_CYAN "Debug::Copper: %d\n" CON_RESET, value.copper);

    return true;
}

static bool cmd_debug_copy_to_empty(void) {
    Entry *entry = entry_get_by_entry_id(E_EMPTY);
    if (entry != NULL) {
        printf(CON_CYAN "Debug::Empty Entry -> %s\n" CON_RESET, entry->short_description);
    }

    Coins coins = {0,5,25};
    entry->id = 5000;
    entry->short_description = "sword";
    entry->traits = "stout sword";
    entry->location = EP_INVENTORY;
    entry->heft = 5;
    entry->damage = d1d8;
    entry->value = coins;
    return true;
}
#endif
