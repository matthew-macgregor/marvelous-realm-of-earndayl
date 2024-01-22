#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "directions.h"
#include "dice.h"

typedef struct Coins {
    int copper;
    int silver;
    int gold;
} Coins;

typedef long entry_id;
typedef struct Entry {
   const entry_id id;
   const char *short_description;
   const char *traits;
   struct Entry *location;
   int heft;
   DiceRoll *damage;
   Coins value;
} Entry;
typedef Entry *EntryArrayPtr;

#define VALUE_NIL          {0,0,0}
#define ENTRY_UNKNOWN      ((entry_id)-1)
#define IS_STATIC          ((int)-1)

// Defined in game/game_data.c
size_t entry_get_entry_count(void);
EntryArrayPtr entry_get_entries(void);

// Defined in entry.c
Entry *entry_get_start_entry(void);
entry_id entry_get_start_entry_id(void);
Entry *entry_search_by_trait(const char *trait);
Entry *entry_search_by_trait_and_entry_id(const char *trait, entry_id loc);
Entry *entry_get_by_entry_id(entry_id location_idx);
bool entry_assign_a_to_b(Entry *a, Entry *b);
bool entry_has_trait(const Entry *entry, const char *trait);
const char *entry_get_short_description(const Entry *entry);
const char *entry_get_traits(const Entry *entry);
const char *entry_get_exits(const Entry *entry);
Coins *entry_get_value(Entry *entry);
bool entry_is_value_nil(Entry *entry);

/**
 * Notes
 * -----
 * An Entry is a generic entity that has several uses in the game:
 * 
 * 1. An Entry acts as a "location". Example: cavern, outhouse, etc.
 * 2. As a "virtual" container, an Entry is a sort of subset of the concept
 *    of a location. A chest can be in a cavern, and a rusty dagger can be in
 *    a chest.
 * 
 *    Cavern [Entry] <- Chest [Entry] <- Rusty dagger [Entry]
 * 
 * 3. As a placeholder for anything which belongs in the game world, which
 *    may be supplemented by data from another table.
 * 
 */

#ifdef TEST
extern int entry_test_main(void);
#endif // end TEST
#endif // ENTRY_H
