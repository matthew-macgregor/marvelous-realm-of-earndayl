#include "private/hero.h"
#include "hero.h"
#include "entry.h"

static Hero hero;

Hero *hero_init(void) {
    hero.location = 0;
    return &hero;
}

Hero *hero_get_hero(void) { 
    return &hero;
}

location_id hero_get_location(void) {
    return hero.location;
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
    mu_assert("hero_init", hero_init()->location == 0);
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
