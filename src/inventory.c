#include "inventory.h"

extern inline bool inv_add_object_to_inventory(Entry *obj) {
    return entry_assign_a_to_b(obj, EP_INVENTORY);
}

extern inline bool inv_remove_object_from_inventory(Entry *obj, Entry *entry) {
    return entry_assign_a_to_b(obj, entry);
}

int inv_get_encumbrance(void) {
    EntryArrayPtr entries = entry_get_entries();
    size_t entry_count = entry_get_entry_count();
    int encumbrance = 0;
    for (size_t i = 0; i < entry_count; i++) {
        Entry entry = entries[i];
        if (inv_contains_entry(&entry) && entry.heft >= 0) {
            encumbrance += entry.heft;
        }
    }

    return encumbrance;
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
    Entry *entry = entry_get_by_entry_id(E_ENTRY_CAVE);
    DiceRoll r = {1,2,0,1};
    Entry obj = { 55, "a lantern", "rusty", entry, 1, &r, {0,0,0}};
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
