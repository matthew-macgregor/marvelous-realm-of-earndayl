#include "entries_data.h"

Entry entries[] = {
   {E_ENTRY_CAVE, "a narrow cave with wet walls", "start" },
   {E_EASTERN_PASSAGE, "the eastern passage", "cave;grotto" },
   {E_WESTERN_PASSAGE, "the western passage", "river;filthy" }
};

#define ENTRY_COUNT (size_t)(sizeof entries / sizeof *entries)

extern size_t entry_get_entry_count(void) {
    return ENTRY_COUNT;
}
