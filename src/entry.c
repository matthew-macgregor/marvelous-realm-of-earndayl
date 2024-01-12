#include <stdbool.h>
#include "entry.h"

#ifdef TEST
#include "entries_test.h"
#else
#include "entries_gen.h"
#endif

#define ENTRY_COUNT (sizeof entries / sizeof *entries)

Entry *entry_search_by_trait(const char *trait) {
    for (size_t i = 0; i < ENTRY_COUNT; i++) {
        Entry entry = entries[i];
        if (strstr(entry.traits, trait)) {
            return &entries[i];
        }
    }

    return NULL;
}

Entry *entry_get_by_location(unsigned long location_idx) {
    if (location_idx < ENTRY_COUNT) {
        return &entries[location_idx];
    }
    return NULL;
}

bool entry_has_trait(Entry *entry, const char *trait) {
    if (entry == NULL) return false;
    return strcmp(entry->traits, trait) == 0;
}

const char *entry_get_description(Entry *entry) {
    return entry != NULL ? entry->description : "unknown";
}

const char *entry_get_traits(Entry *entry) {
    return entry != NULL ? entry->traits : "unknown";
}

#ifdef TEST
#include "output.h"
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

static char *test_search_by_trait() {
    mu_assert("search by trait 'cave'", entry_search_by_trait("cave") == &entries[1]);
    mu_assert("search by trait 'grotto'", entry_search_by_trait("grotto") == &entries[2]);
    mu_assert("search by trait 'bogus'", entry_search_by_trait("bogus") == NULL);
    return 0;
}

static char *test_entry_count() {
    mu_assert("entry count", ENTRY_COUNT == 3);
    return 0;
}

static char *test_entry_get_by_location() {
    mu_assert("entry_get_by_location", entry_has_trait(entry_get_by_location(0), "start"));
    return 0;
}

static char *test_entry_get_description_trait() {
    Entry *entry = entry_get_by_location(0);
    mu_assert("entry_get_description", strcmp(entry_get_description(entry), "a narrow cave with wet walls") == 0);
    mu_assert("entry_get_traits", strcmp(entry_get_traits(entry), "start") == 0);
    return 0;
}

static char *entry_test_all_tests() {
    mu_run_test(test_search_by_trait);
    mu_run_test(test_entry_count);
    mu_run_test(test_entry_get_by_location);
    mu_run_test(test_entry_get_description_trait);

    // next test here
    return 0;
}

int entry_test_main() {
    const char *result = entry_test_all_tests();
    return report_test_results("Entry", result);
}
#endif // TEST