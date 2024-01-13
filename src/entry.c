#include <stdbool.h>
#include <stdio.h>
#include "entry.h"
#include "string_utils.h"

#ifdef TEST
#include "entries_test.h"
#else
#include "entries_gen.h"
#endif

#define ENTRY_COUNT (long)(sizeof entries / sizeof *entries)

// -------------------------             N  / E  / S  / W    NE  / NW  / SE /  SW
static char* exit_letter_mapping[8] = { "N", "E", "S", "W", "NE", "NW", "SE", "SW" };

extern long entry_get_entry_count(void) {
    return ENTRY_COUNT;
}

Entry *entry_search_by_trait(const char *trait) {
    for (size_t i = 0; i < ENTRY_COUNT; i++) {
        Entry entry = entries[i];
        if (strstr(entry.traits, trait)) {
            return &entries[i];
        }
    }

    return NULL;
}

Entry *entry_get_by_location(location_id location_idx) {
    if (location_idx < ENTRY_COUNT) {
        return &entries[location_idx];
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

bool entry_is_direction_ok(const Entry *entry, const Direction direction) {
    return is_valid_direction(direction) ? (entry->exits[direction].location) > 0 : false;
}

location_id entry_get_location_in_direction(const Entry *entry, const Direction direction) {
    return is_valid_direction(direction) ? entry->exits[direction].location : LOCATION_UNKNOWN;
}

const char *entry_get_exits(const Entry *entry) {
    static char exits[25]; // "N/W/E/S/NE/NW/SE/SW"
    exits[0] = '\0';
    bool at_least_one = false;
    for (unsigned long i = 0; i < 8; i++) {
        if (entry->exits[i].location >= 0) {
            if (at_least_one) {
                strncat(exits, " / ", 3);
            }
            char *exit_name = exit_letter_mapping[i];
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
    mu_assert("search by trait 'cave'", entry_search_by_trait("cave") == &entries[1]);
    mu_assert("search by trait 'grotto'", entry_search_by_trait("grotto") == &entries[2]);
    mu_assert("search by trait 'bogus'", entry_search_by_trait("bogus") == NULL);
    return 0;
}

static char *test_entry_count(void) {
    mu_assert("entry count", ENTRY_COUNT == 3);
    return 0;
}

static char *test_entry_get_by_location(void) {
    mu_assert("entry_get_by_location", entry_has_trait(entry_get_by_location(0), "start"));
    return 0;
}

static char *test_entry_get_description_trait(void) {
    Entry *entry = entry_get_by_location(0);
    mu_assert("entry_get_description", strcmp(entry_get_description(entry), "a narrow cave with wet walls") == 0);
    mu_assert("entry_get_traits", strcmp(entry_get_traits(entry), "start") == 0);
    return 0;
}

static char *test_entry_get_exits(void) {
    Entry *entry = entry_get_by_location(0);
    mu_assert("entry_get_exits", strcmp(entry_get_exits(entry), "W / SW") == 0);
    entry = entry_get_by_location(1);
    mu_assert("entry_get_exits", strcmp(entry_get_exits(entry), "E / SW") == 0);
    return 0;
}

static char *test_entry_is_direction_ok(void) {
    Entry *entry = entry_get_by_location(0);
    mu_assert("entry_is_direction_ok: 0, west", entry_is_direction_ok(entry, WEST));
    mu_assert("entry_is_direction_ok: 0, east", entry_is_direction_ok(entry, EAST) == false);
    mu_assert("entry_is_direction_ok: 0, bogus", entry_is_direction_ok(entry, DIRECTION_UNKNOWN) == false);
    mu_assert("entry_is_direction_ok: 0, bogus again", entry_is_direction_ok(entry, 500) == false);
    mu_assert("entry_is_direction_ok: 0, west", entry_is_direction_ok(entry, SOUTHWEST));

    return 0;
}

static char *test_entry_get_location_in_direction(void) {
    Entry *entry = entry_get_by_location(0);
    mu_assert("entry_get_location_in_direction", entry_get_location_in_direction(entry, WEST) == 1);
    return 0;
}


static char *entry_test_all_tests(void) {
    mu_run_test(test_search_by_trait);
    mu_run_test(test_entry_count);
    mu_run_test(test_entry_get_by_location);
    mu_run_test(test_entry_get_description_trait);
    mu_run_test(test_entry_get_exits);
    mu_run_test(test_entry_is_direction_ok);
    mu_run_test(test_entry_get_location_in_direction);

    // next test here
    return 0;
}

int entry_test_main(void) {
    const char *result = entry_test_all_tests();
    return report_test_results("Entry", result);
}
#endif // TEST