#include "game/game_data.h"
#include "dice.h"

DiceRoll range_1d2 = dice_new_die(1, 2);

Entry entries[] = {
    // entry_id, short_description, traits, entry, heft, damage (min, max)
    {E_INVENTORY, "your backpack", "inventory;backpack", NULL, IS_STATIC, NULL, VALUE_NIL},
    {E_ENTRY_CAVE, "a narrow cave with wet walls", "start", NULL, IS_STATIC, NULL, VALUE_NIL},
    {E_EASTERN_PASSAGE, "the eastern passage", "eastern passage", NULL, IS_STATIC, NULL, VALUE_NIL},
    {E_WESTERN_PASSAGE, "the western passage", "western passage", NULL, IS_STATIC, NULL, VALUE_NIL},
    {E_ROTTEN_CHEST, "a rotten chest", "rotten chest", EP_WESTERN_PASSAGE, 20, NULL, VALUE_NIL},
    {E_RUSTY_DAGGER, "a rusty dagger", "rusty dagger", EP_ROTTEN_CHEST, 1, &range_1d2, {5,0,0}},
    {E_COPPER_PENNY, "a copper penny", "copper penny", EP_ENTRY_CAVE, 0, NULL, {1,0,0}}
};

Connector connectors[] = {
    { EP_ENTRY_CAVE, EAST, EP_EASTERN_PASSAGE },
    { EP_EASTERN_PASSAGE, WEST, EP_ENTRY_CAVE },
    { EP_ENTRY_CAVE, WEST, EP_WESTERN_PASSAGE },
    { EP_WESTERN_PASSAGE, EAST, EP_ENTRY_CAVE }
};

#define ENTRY_COUNT (size_t)(sizeof entries / sizeof *entries)
#define CONNECTOR_COUNT (size_t)(sizeof connectors / sizeof *connectors)
#define TREASURE_VALUE_COUNT (size_t)(sizeof treasure_values / sizeof *treasure_values)

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
