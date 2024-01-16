#ifndef OBJECTS_H
#define OBJECTS_H
#include <stddef.h>
#include "entry.h"

typedef long object_id;

typedef struct Object {
    object_id id;
    char *short_description;
    char *traits;
    Entry *location;
} Object;
typedef Object* ObjectArrayPtr;

ObjectArrayPtr obj_get_objects(void);
size_t obj_get_object_count(void);
Object *obj_search_by_trait(const char *trait);
Object *obj_search_by_trait_and_entry_id(const char *trait, entry_id loc);
Object *obj_find_by_id(object_id id);
bool obj_move_object(Object *obj, Entry *entry);

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
