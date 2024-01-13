#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Entry Entry;
typedef unsigned long location_id;

Entry *entry_search_by_trait(const char *trait);
Entry *entry_get_by_location(location_id location_idx);
bool entry_has_trait(const Entry *entry, const char *trait);
const char *entry_get_description(const Entry *entry);
const char *entry_get_traits(const Entry *entry);
const char *entry_get_exits(const Entry *entry);

#ifdef TEST
extern int entry_test_main(void);
#endif // end TEST
#endif // ENTRY_H
