#include "treasure.h"

EntryValue *treasure_get_value(Entry *entry) {
    EntryValueArrayPtr treasures = treasure_get_treasures();
    size_t count = treasure_get_treasures_count();
    for (size_t i = 0; i < count; i++) {
        EntryValue *value = &treasures[i];
        if (value != NULL && value->entry == entry) {
            return value;
        }
    }

    return NULL;
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "test_results.h"

static char *treasure_test_baseline(void) {
    return 0;
}

static char *treasure_test_all_tests(void) {
    mu_run_test(treasure_test_baseline);

    // next test here
    return 0;
}

int treasure_test_main(void) {
    const char *result = treasure_test_all_tests();
    return report_test_results("Sample", result);
}
#endif // TEST
