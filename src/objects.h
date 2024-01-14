#ifndef OBJECTS_H
#define OBJECTS_H
#include <stddef.h>
#include "entry.h"

typedef long object_id;

typedef struct Object {
    object_id id;
    char *description;
    char *traits;
    Entry *location;
} Object;
typedef Object* ObjectArrayPtr;

ObjectArrayPtr obj_get_objects(void);
size_t obj_get_connector_count(void);
Object *obj_search_by_trait(const char *trait);
Object *obj_find_by_id(object_id id);

/**
 * Notes
 * -----
 * An object covers most of the non-animate items in the game: lamp, knife,
 * rope, chest, etc.
 * 
 * I considered Ruud Helderman's approach (https://helderman.github.io/htpataic/htpataic04.html)
 * of making everything (including rooms) an object, but decided against it.
 */

#ifdef TEST
int objects_test_main(void);
#endif // TEST
#endif // OBJECTS_H
