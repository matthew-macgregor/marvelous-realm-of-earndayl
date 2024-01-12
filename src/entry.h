#ifndef ENTRY_H
#define ENTRY_H

#include <stdlib.h>
#include <string.h>
#include "output.h"

typedef struct Entry Entry;

Entry *entry_search_by_trait(const char *trait);
Entry *entry_get_by_location(unsigned long location_idx);
bool entry_has_trait(Entry *entry, const char *trait);
const char *entry_get_description(Entry *entry);
const char *entry_get_traits(Entry *entry);

#ifdef TEST
extern int entry_test_main();
#endif // end TEST
#endif // ENTRY_H