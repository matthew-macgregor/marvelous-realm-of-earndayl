#ifndef GAME_DATA_ENTRIES_H
#define GAME_DATA_ENTRIES_H
#include "entry.h"
#include "connectors.h"

extern Entry entries[];

// Entry IDs: Each must be unique and stable (for save games)
#define E_ENTRY_CAVE            100
#define E_EASTERN_PASSAGE       101
#define E_WESTERN_PASSAGE       102

// Entry Pointers (EP): Used internally to reference the entries directly
#define EP_ENTRY_CAVE           (entries + 0)
#define EP_EASTERN_PASSAGE      (entries + 1)
#define EP_WESTERN_PASSAGE      (entries + 2)

#endif // GAME_DATA_ENTRIES_H
