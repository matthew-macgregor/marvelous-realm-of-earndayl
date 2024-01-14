#ifndef GAME_DATA_ENTRIES_H
#define GAME_DATA_ENTRIES_H
#include "entry.h"
#include "private/entry.h"
#include "entry_ids.h"
#include "connectors.h"

Entry entries[] = {
   {E_ENTRY_CAVE, "a narrow cave with wet walls", "start" },
   {E_EASTERN_PASSAGE, "the eastern passage", "cave" },
   {E_WESTERN_PASSAGE, "the western passage", "river;filthy" }
};

#define ENTRY_COUNT (long)(sizeof entries / sizeof *entries)

#endif // GAME_DATA_ENTRIES_H
