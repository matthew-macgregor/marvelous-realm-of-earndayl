#ifndef ENTRIES_GEN_H
#define ENTRIES_GEN_H

#include "entry.h"
#include "private/entry.h"

// ---------------------- N/E/S/W NE/NW/SE/SW
#define exits_z { -1,-1,-1,-1, -1,-1,-1,-1 }
#define exits_0 { -1,-1,-1, 1, -1,-1,-1,-1 }

static Entry entries[] = {
   {"a narrow cave with wet walls", "start", exits_0 },
   {"the eastern passage", "cave", exits_z},
   {"the western passage", "river;filthy", exits_z}
};

#endif // ENTRIES_GEN_H
