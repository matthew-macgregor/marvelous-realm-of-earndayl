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

Object *obj_search_by_trait_and_location_id(const char *trait, location_id loc) {
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

#ifdef TEST
#include "colors.h"
#include "game/game_data.h"
#include "minunit.h"
#include "test_results.h"

static char *test_obj_baseline(void) {
    ObjectArrayPtr objects = obj_get_objects();
    mu_assert("obj_search_by_trait", obj_search_by_trait("rusty") ==  &objects[0]);
    return 0;
}

static char *test_obj_search_by_trait_and_location_id(void) {
    mu_assert("obj_search_by_trait_and_location_id", obj_search_by_trait_and_location_id("rusty", E_ENTRY_CAVE) ==  &objects[0]);
    return 0;
}

static char *objects_test_all_tests(void) {
    mu_run_test(test_obj_baseline);
    mu_run_test(test_obj_search_by_trait_and_location_id);

    // next test here
    return 0;
}

int objects_test_main(void) {
    const char *result = objects_test_all_tests();
    return report_test_results("Objects", result);
}
#endif // TEST
