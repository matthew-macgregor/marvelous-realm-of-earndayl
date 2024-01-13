#ifndef ENTRIES_H
#define ENTRIES_H

#include "entry.h"
#include "private/entry.h"

// -------------    N /  E /  S /  W    NE / NW / SE / SW
#define exits_z { {-1},{-1},{-1},{-1}, {-1},{-1},{-1},{-1} }
#define exits_0 { {-1},{-1},{-1}, {1}, {-1},{-1},{-1}, {2} }
#define exits_1 { {-1},{0},{-1}, {-1}, {-1},{-1},{-1}, {2} }

Entry entries[] = {
   {"a narrow cave with wet walls", "start", exits_0},
   {"the eastern passage", "cave", exits_1},
   {"the western passage", "grotto;filthy", exits_z}
};

#endif // ENTRIES_H
