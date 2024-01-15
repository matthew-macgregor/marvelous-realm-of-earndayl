#include "inventory.h"

extern inline bool inv_add_object_to_inventory(Object *obj) {
    if (obj != NULL) {
        obj->location = EP_INVENTORY;
        return true;
    }

    return false;
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "test_results.h"
#include "entry.h"

static char *test_inv_baseline(void) {
    mu_assert("inv_get_inventory_entry", inv_get_inventory_entry() == EP_INVENTORY);
    return 0;
}

static char *test_inv_add_object_to_inventory(void) {
    Entry *entry = entry_get_start_entry();
    Object obj = { 55, "a lantern", "rusty", entry };
    inv_add_object_to_inventory(&obj);
    mu_assert("inv_add_object_to_inventory", obj.location == EP_INVENTORY); 
    return 0;
}

static char *inv_test_all_tests(void) {
    mu_run_test(test_inv_baseline);
    mu_run_test(test_inv_add_object_to_inventory);

    // next test here
    return 0;
}

int inventory_test_main(void) {
    const char *result = inv_test_all_tests();
    return report_test_results("Inventory", result);
}
#endif // TEST
