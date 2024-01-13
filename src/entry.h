#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "directions.h"

typedef struct Entry Entry;
typedef long location_id;

#define LOCATION_UNKNOWN ((location_id)-1)

extern long entry_get_entry_count(void);
Entry *entry_search_by_trait(const char *trait);
Entry *entry_get_by_location(location_id location_idx);
bool entry_has_trait(const Entry *entry, const char *trait);
const char *entry_get_description(const Entry *entry);
const char *entry_get_traits(const Entry *entry);
const char *entry_get_exits(const Entry *entry);
bool entry_is_direction_ok(const Entry *entry, const Direction direction);
location_id entry_get_location_in_direction(const Entry *entry, const Direction direction);

#ifdef TEST
extern int entry_test_main(void);
#endif // end TEST
#endif // ENTRY_H