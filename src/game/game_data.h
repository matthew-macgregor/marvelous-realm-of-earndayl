#ifndef GAME_DATA_ENTRIES_H
#define GAME_DATA_ENTRIES_H
#include "entry.h"
#include "connectors.h"

extern Entry entries[];
extern Connector connectors[];

// Entry IDs: Each must be unique and stable (for save games)
#define E_INVENTORY                  99
#define E_ENTRY_CAVE                100
#define E_EASTERN_PASSAGE           101
#define E_WESTERN_PASSAGE           102

// Objects
#define E_ROTTEN_CHEST              1000
#define E_RUSTY_DAGGER              1001
#define E_COPPER_PENNY              1002

// Entry Pointers (EP): Used internally to reference the entries directly
#define EP_INVENTORY                (entries + 0)
#define EP_ENTRY_CAVE               (entries + 1)
#define EP_EASTERN_PASSAGE          (entries + 2)
#define EP_WESTERN_PASSAGE          (entries + 3)
#define EP_ROTTEN_CHEST             (entries + 4)
#define EP_RUSTY_DAGGER             (entries + 5)
#define EP_COPPER_PENNY             (entries + 6)

#endif // GAME_DATA_ENTRIES_H
