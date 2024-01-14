#ifndef ENTRIES_H
#define ENTRIES_H

#include "entry.h"
#include "private/entry.h"
#include "entry_ids_test.h"

Entry entries[] = {
   {E_ENTRY_CAVE, "a narrow cave with wet walls", "start" },
   {E_EASTERN_PASSAGE, "the eastern passage", "cave" },
   {E_WESTERN_PASSAGE, "the western passage", "grotto;filthy" }
};

#define ENTRY_COUNT (long)(sizeof entries / sizeof *entries)

#endif // ENTRIES_H
