#include "hero.h"
#include "entry.h"

static Hero hero;

bool hero_arm_weapon(Weapon *weapon) {
    if (weapon->damage != NULL) {
        hero.armed_weapon = weapon;
        return true;
    }

    return false;
}

Hero *hero_init(void) {
    hero.location = entry_get_start_entry();
    return &hero;
}

Hero *hero_get_hero(void) { 
    return &hero;
}

entry_id hero_get_entry_id(void) {
    return hero.location != NULL ? hero.location->id : ENTRY_UNKNOWN;
}

Entry *hero_get_entry(void) {
    return hero.location != NULL ? hero.location : NULL;
}

bool hero_set_entry_id(entry_id new_loc_id) {
    Entry *entry = entry_get_by_entry_id(new_loc_id);
    if (entry != NULL) {
        hero.location = entry;
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

static char *test_hero_get_hero(void) {
    mu_assert("hero_get_hero", hero_get_hero() == &hero);
    return 0;
}

static char *test_hero_init(void) {
    mu_assert("hero_init", hero_init()->location == entry_get_start_entry());
    return 0;
}

static char *test_hero_get_entry(void) {
    mu_assert("hero_get_entry", hero_get_entry() == hero.location);
    return 0;
}

static char *hero_test_all_tests(void) { 
    mu_run_test(test_hero_get_hero);
    mu_run_test(test_hero_init);
    mu_run_test(test_hero_get_entry);

    // next test here
    return 0;
}

int hero_test_main(void) {
    const char *result = hero_test_all_tests();
    return report_test_results("Hero", result);
}

#endif
