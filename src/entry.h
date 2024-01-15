#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "directions.h"
#include "location.h"

typedef struct Entry {
   const location_id id;
   const char *short_description;
   const char *traits;
} Entry;
typedef Entry* EntryArrayPtr;

// Defined in game/game_data.c
size_t entry_get_entry_count(void);
EntryArrayPtr entry_get_entries(void);

// Defined in entry.c
Entry *entry_get_start_entry(void);
location_id entry_get_start_location_id(void);
Entry *entry_search_by_trait(const char *trait);
Entry *entry_get_by_location_id(location_id location_idx);
bool entry_has_trait(const Entry *entry, const char *trait);
const char *entry_get_short_description(const Entry *entry);
const char *entry_get_traits(const Entry *entry);
const char *entry_get_exits(const Entry *entry);

#ifdef TEST
extern int entry_test_main(void);
#endif // end TEST
#endif // ENTRY_H
