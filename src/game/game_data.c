#include "game/game_data.h"

Entry entries[] = {
   {E_INVENTORY, "your backpack", "inventory;backpack" },
   {E_ENTRY_CAVE, "a narrow cave with wet walls", "start" },
   {E_EASTERN_PASSAGE, "the eastern passage", "cave;grotto" },
   {E_WESTERN_PASSAGE, "the western passage", "river;filthy" }
};

Connector connectors[] = {
    { EP_ENTRY_CAVE, EAST, EP_EASTERN_PASSAGE },
    { EP_EASTERN_PASSAGE, WEST, EP_ENTRY_CAVE },
    { EP_ENTRY_CAVE, WEST, EP_WESTERN_PASSAGE },
    { EP_WESTERN_PASSAGE, EAST, EP_ENTRY_CAVE }
};

Object objects[] = {
    { O_RUSTY_DAGGER, "a rusty dagger", "rusty dagger", EP_ENTRY_CAVE }
};

#define ENTRY_COUNT (size_t)(sizeof entries / sizeof *entries)
#define CONNECTOR_COUNT (long)(sizeof connectors / sizeof *connectors)
#define OBJECT_COUNT (long)(sizeof objects / sizeof *objects)

extern inline size_t entry_get_entry_count(void) {
    return ENTRY_COUNT;
}

extern inline EntryArrayPtr entry_get_entries(void) {
    return entries;
}

extern inline size_t connector_get_connector_count(void) {
    return CONNECTOR_COUNT;
}

extern inline ConnectorArrayPtr connector_get_connectors(void) {
    return connectors;
}

extern inline size_t obj_get_object_count(void) {
    return OBJECT_COUNT;
}

extern inline ObjectArrayPtr obj_get_objects(void) {
    return objects;
}

/**
 * Implementation Notes
 * --------------------
 * Note on use of extern inline. By making the definition extern inline but the
 * declaration (see header) not inline, this function should be the equivalent
 * of a macro.
 * 
 * > If you specify both inline and extern in the function definition, then the
 * > definition is used only for inlining. In no case is the function compiled
 * > on its own, not even if you refer to its address explicitly. Such an 
 * > address becomes an external reference, as if you had only declared the
 * > function, and had not defined it.
 * 
 * https://gcc.gnu.org/onlinedocs/gcc/Inline.html 
 */
