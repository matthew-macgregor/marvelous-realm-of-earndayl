#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "entry.h"
#include "connectors.h"
#include "directions.h"
#include "game/game_data.h"

// -------------------------             N  / E  / S  / W    NE  / NW  / SE /  SW
static char* exit_letter_mapping[8] = { "N", "E", "S", "W", "NE", "NW", "SE", "SW" };

// extern Entry entries[];

Entry *entry_get_start_entry(void) {
    return entry_search_by_trait("start");
}

entry_id entry_get_start_location_id(void) {
    Entry *entry = entry_get_start_entry();
    return entry != NULL ? entry->id : ENTRY_UNKNOWN;
}

Entry *entry_search_by_trait(const char *trait) {
    size_t entry_cnt = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    for (size_t i = 0; i < entry_cnt; i++) {
        Entry entry = entries[i];
        printf("%zu) %s -> %s\n", i, entry.traits, trait);
        if (strstr(entry.traits, trait)) {
            printf(">> %s -> %s\n", entry.traits, trait);
            return &entries[i];
        }
    }

    return NULL;
}

Entry *entry_search_by_trait_and_entry_id(const char *trait, entry_id loc) {
    size_t count = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    for (size_t i = 0; i < count; i++) {
        Entry e = entries[i];
        if (strstr(e.traits, trait) &&
            e.location != NULL &&
            e.location->id == loc) {
            return &entries[i];
        }
    }

    return NULL;
}

Entry *entry_get_by_entry_id(entry_id entry_id) {
    size_t entry_cnt = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    for (size_t i = 0; i < entry_cnt; i++) {
        if (entries[i].id == entry_id) {
            return &entries[i];
        }
    }

    return NULL;
}

bool entry_has_trait(const Entry *entry, const char *trait) {
    if (entry == NULL) return false;
    return strcmp(entry->traits, trait) == 0;
}

const char *entry_get_short_description(const Entry *entry) {
    return entry != NULL ? entry->short_description : "unknown";
}

const char *entry_get_traits(const Entry *entry) {
    return entry != NULL ? entry->traits : "unknown";
}

const char *entry_get_exits(const Entry *entry) {
    if (entry == NULL) return "Unknown Exits";
    static char exits[25]; // "N/W/E/S/NE/NW/SE/SW"
    exits[0] = '\0';
    bool at_least_one = false;
    for (Direction dir = 0; dir < 8; dir++) {
        entry_id loc = connector_get_location_id_in_direction(entry, dir);
        if (loc > ENTRY_UNKNOWN) {
            if (at_least_one) {
                strncat(exits, "/", 3);
            }
            char *exit_name = exit_letter_mapping[dir];
            strncat(exits, exit_name, strlen(exit_name));
            at_least_one = true;
        }
    }
    return exits;
}

extern inline bool entry_assign_a_to_b(Entry *a, Entry *b) {
    if (a != NULL) {
        a->location = b;
        return true;
    }
    return false;
}

bool entry_is_value_nil(Entry *entry) {
    Coins *coins = &entry->value;
    return (
        coins->copper   == 0 &&
        coins->silver   == 0 &&
        coins->gold     == 0
    );
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

static char *test_search_by_trait(void) {
    EntryArrayPtr entries = entry_get_entries();
    mu_assert("search by trait 'start'", entry_search_by_trait("start") == EP_ENTRY_CAVE);
    mu_assert("search by trait 'passage'", entry_search_by_trait("passage") == EP_EASTERN_PASSAGE);
    mu_assert("search by trait 'bogus'", entry_search_by_trait("bogus") == NULL);
    return 0;
}

static char *test_entry_search_by_trait_and_location_id(void) {
    mu_assert("entry_search_by_trait_and_location_id", entry_search_by_trait_and_entry_id("rotten", E_WESTERN_PASSAGE) == EP_ROTTEN_CHEST);
    return 0;
}

static char *test_entry_move_entry(void) {
    Entry *container = entry_get_by_entry_id(E_ENTRY_CAVE);
    DiceRoll r = {1, 2, 0, 1};
    Entry contained = { 55, "a lantern", "rusty", NULL, 1, &r, {0,0,0}};
    bool result = entry_assign_a_to_b(&contained, container);
    mu_assert("entry_move_entry: result", result);
    mu_assert("entry_move_entry: entry", contained.location == EP_ENTRY_CAVE);
    return 0;
}

static char *test_entry_count(void) {
    size_t entry_cnt = entry_get_entry_count();
    mu_assert("entry count", entry_cnt == 10);
    return 0;
}

static char *test_entry_get_by_entry_id(void) {
    mu_assert("entry_get_by_location", entry_has_trait(entry_get_by_entry_id(E_ENTRY_CAVE), "start"));
    return 0;
}

static char *test_entry_get_description_trait(void) {
    Entry *entry = entry_get_by_entry_id(E_ENTRY_CAVE);
    mu_assert("entry_get_short_description", strcmp(entry_get_short_description(entry), "a narrow cave with wet walls") == 0);
    mu_assert("entry_get_traits", strcmp(entry_get_traits(entry), "start") == 0);
    return 0;
}

static char *test_entry_get_exits(void) {
    Entry *entry = entry_get_by_entry_id(E_ENTRY_CAVE);
    mu_assert("entry_get_exits: E_ENTRY_CAVE", strcmp(entry_get_exits(entry), "E/W") == 0);
    entry = entry_get_by_entry_id(E_EASTERN_PASSAGE);
    mu_assert("entry_get_exits: E_EASTERN_PASSAGE", strcmp(entry_get_exits(entry), "W") == 0);
    return 0;
}

static char *entry_test_all_tests(void) {
    mu_run_test(test_search_by_trait);
    mu_run_test(test_entry_count);
    mu_run_test(test_entry_get_by_entry_id);
    mu_run_test(test_entry_get_description_trait);
    mu_run_test(test_entry_get_exits);
    mu_run_test(test_entry_search_by_trait_and_location_id);
    mu_run_test(test_entry_move_entry);

    // next test here
    return 0;
}

int entry_test_main(void) {
    const char *result = entry_test_all_tests();
    return report_test_results("Entry", result);
}
#endif // TEST
