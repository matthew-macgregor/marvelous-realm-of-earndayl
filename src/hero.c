#include "hero.h"
#include "entry.h"

static Hero hero;

Hero *hero_init(void) {
    hero.location = entry_get_start_location_id();
    return &hero;
}

Hero *hero_get_hero(void) { 
    return &hero;
}

location_id hero_get_location_id(void) {
    return hero.location;
}

bool hero_set_location_id(location_id new_loc_id) {
    Entry *entry = entry_get_by_location_id(new_loc_id);
    if (entry != NULL) {
        hero.location = new_loc_id;
        return true;
    }

    return false;
}

// TESTS
// ==================================================================
#ifdef TEST
#include <stdbool.h>
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

static char *hero_test_get_hero(void) {
    mu_assert("hero_get_hero", hero_get_hero() == &hero);
    return 0;
}

static char *hero_test_init(void) {
    mu_assert("hero_init", hero_init()->location == 100);
    return 0;
}

static char *hero_test_all_tests(void) { 
    mu_run_test(hero_test_get_hero);
    mu_run_test(hero_test_init);

    // next test here
    return 0;
}

int hero_test_main(void) {
    const char *result = hero_test_all_tests();
    return report_test_results("Hero", result);
}

#endif
