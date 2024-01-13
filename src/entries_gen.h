#ifndef ENTRIES_GEN_H
#define ENTRIES_GEN_H

#include "entry.h"
#include "private/entry.h"

// ---------------------- N/E/S/W NE/NW/SE/SW
#define exits_z { {-1},{-1},{-1},{-1}, {-1},{-1},{-1},{-1} }
#define exits_0 { {-1},{-1},{-1}, {2}, {-1},{-1},{-1},{-1} }
#define exits_2 { {-1},{0},{-1}, {-1}, {-1},{-1},{-1},{-1} }

static Entry entries[] = {
   {"a narrow cave with wet walls", "start", exits_0 },
   {"the eastern passage", "cave", exits_z },
   {"the western passage", "river;filthy", exits_2 }
};

#endif // ENTRIES_GEN_H
