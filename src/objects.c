#include <stddef.h>
#include <string.h>
#include "objects.h"

Object *obj_search_by_trait(const char *trait) {
    size_t count = obj_get_object_count();
    ObjectArrayPtr objects = obj_get_objects();
    for (size_t i = 0; i < count; i++) {
        Object o = objects[i];
        if (strstr(o.traits, trait)) {
            return &objects[i];
        }
    }

    return NULL;
}

Object *obj_search_by_trait_and_entry_id(const char *trait, entry_id loc) {
    size_t count = obj_get_object_count();
    ObjectArrayPtr objects = obj_get_objects();
    for (size_t i = 0; i < count; i++) {
        Object o = objects[i];
        if (strstr(o.traits, trait) && 
            o.location != NULL && 
            o.location->id == loc) {
            return &objects[i];
        }
    }

    return NULL;
}

Object *obj_find_by_id(object_id id) {
    size_t count = obj_get_object_count();
    ObjectArrayPtr objects = obj_get_objects();
    for (size_t i = 0; i < count; i++) {
        Object o = objects[i];
        if (o.id == id) {
            return &objects[i];
        }
    }

    return NULL;
}

extern inline bool obj_move_object(Object *obj, Entry *entry) {
    if (obj != NULL) {
        obj->location = entry;
        return true;
    }

    return false;
}

#ifdef TEST
#include "colors.h"
#include "entry.h"
#include "game/game_data.h"
#include "minunit.h"
#include "test_results.h"

static char *test_obj_baseline(void) {
    ObjectArrayPtr objects = obj_get_objects();
    mu_assert("obj_search_by_trait", obj_search_by_trait("rusty") ==  &objects[0]);
    return 0;
}

static char *test_obj_search_by_trait_and_location_id(void) {
    mu_assert("obj_search_by_trait_and_location_id", obj_search_by_trait_and_entry_id("rusty", E_ENTRY_CAVE) ==  &objects[0]);
    return 0;
}

static char *test_obj_move_obj(void) {
    Entry *entry = entry_get_by_entry_id(E_ENTRY_CAVE);
    Object obj = { 55, "a lantern", "rusty", entry };
    bool result = obj_move_object(&obj, entry);
    mu_assert("obj_move_object: result", result);
    mu_assert("obj_move_object: entry", obj.location == EP_ENTRY_CAVE);
    return 0;
}

static char *objects_test_all_tests(void) {
    mu_run_test(test_obj_baseline);
    mu_run_test(test_obj_search_by_trait_and_location_id);
    mu_run_test(test_obj_move_obj);

    // next test here
    return 0;
}

int objects_test_main(void) {
    const char *result = objects_test_all_tests();
    return report_test_results("Objects", result);
}
#endif // TEST
