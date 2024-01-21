#ifndef TREASURE_H
#define TREASURE_H
#include "entry.h"

typedef struct Coins {
    int copper;
    int silver;
    int gold;
} Coins;

typedef struct EntryValue {
    entry_id id;
    Entry *entry;
    Coins coins;
} EntryValue;
typedef EntryValue* EntryValueArrayPtr;

#define TREASURE_EMPTY {0,0,0}

// Defined in game_data.c
EntryValueArrayPtr treasure_get_treasures(void);
size_t treasure_get_treasures_count(void);
// ------
EntryValue *treasure_get_value(Entry *entry);

#ifdef TEST
int treasure_test_main(void);
#endif // TEST
#endif // TREASURE_H
