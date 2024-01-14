#include <stdbool.h>
#include <stdio.h>
#include "entry.h"
#include "string_utils.h"
#include "connectors.h"
#include "game/game_data.h"

// -------------------------             N  / E  / S  / W    NE  / NW  / SE /  SW
static char* exit_letter_mapping[8] = { "N", "E", "S", "W", "NE", "NW", "SE", "SW" };

// extern Entry entries[];

Entry *entry_get_start_entry(void) {
    return entry_search_by_trait("start");
}

location_id entry_get_start_location_id(void) {
    Entry *entry = entry_get_start_entry();
    return entry != NULL ? entry->id : LOCATION_UNKNOWN;
}

Entry *entry_search_by_trait(const char *trait) {
    size_t entry_cnt = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    for (size_t i = 0; i < entry_cnt; i++) {
        Entry entry = entries[i];
        if (strstr(entry.traits, trait)) {
            return &entries[i];
        }
    }

    return NULL;
}

Entry *entry_get_by_location_id(location_id location_idx) {
    size_t entry_cnt = entry_get_entry_count();
    EntryArrayPtr entries = entry_get_entries();
    for (size_t i = 0; i < entry_cnt; i++) {
        if (entries[i].id == location_idx) {
            return &entries[i];
        }
    }

    return NULL;
}

bool entry_has_trait(const Entry *entry, const char *trait) {
    if (entry == NULL) return false;
    return strcmp(entry->traits, trait) == 0;
}

const char *entry_get_description(const Entry *entry) {
    return entry != NULL ? entry->description : "unknown";
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
        location_id loc = connector_get_location_id_in_direction(entry, dir);
        if (loc > LOCATION_UNKNOWN) {
            if (at_least_one) {
                strncat(exits, " / ", 3);
            }
            char *exit_name = exit_letter_mapping[dir];
            strncat(exits, exit_name, strlen(exit_name));
            at_least_one = true;
        }
    }
    return exits;
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

static char *test_search_by_trait(void) {
    EntryArrayPtr entries = entry_get_entries();
    mu_assert("search by trait 'start'", entry_search_by_trait("start") == &entries[0]);
    mu_assert("search by trait 'grotto'", entry_search_by_trait("grotto") == &entries[1]);
    mu_assert("search by trait 'bogus'", entry_search_by_trait("bogus") == NULL);
    return 0;
}

static char *test_entry_count(void) {
    size_t entry_cnt = entry_get_entry_count();
    mu_assert("entry count", entry_cnt == 3);
    return 0;
}

static char *test_entry_get_by_location_id(void) {
    mu_assert("entry_get_by_location", entry_has_trait(entry_get_by_location_id(E_ENTRY_CAVE), "start"));
    return 0;
}

static char *test_entry_get_description_trait(void) {
    Entry *entry = entry_get_by_location_id(E_ENTRY_CAVE);
    mu_assert("entry_get_description", strcmp(entry_get_description(entry), "a narrow cave with wet walls") == 0);
    mu_assert("entry_get_traits", strcmp(entry_get_traits(entry), "start") == 0);
    return 0;
}

static char *test_entry_get_exits(void) {
    Entry *entry = entry_get_by_location_id(E_ENTRY_CAVE);
    mu_assert("entry_get_exits: E_ENTRY_CAVE", strcmp(entry_get_exits(entry), "E / W") == 0);
    entry = entry_get_by_location_id(E_EASTERN_PASSAGE);
    mu_assert("entry_get_exits: E_EASTERN_PASSAGE", strcmp(entry_get_exits(entry), "W") == 0);
    return 0;
}

static char *entry_test_all_tests(void) {
    mu_run_test(test_search_by_trait);
    mu_run_test(test_entry_count);
    mu_run_test(test_entry_get_by_location_id);
    mu_run_test(test_entry_get_description_trait);
    mu_run_test(test_entry_get_exits);

    // next test here
    return 0;
}

int entry_test_main(void) {
    const char *result = entry_test_all_tests();
    return report_test_results("Entry", result);
}
#endif // TEST
