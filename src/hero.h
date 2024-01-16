#ifndef HERO_H
#define HERO_H

#include "entry.h"

typedef struct Hero {
    Entry* location;
} Hero;

Hero *hero_init(void);
Hero *hero_get_hero(void);
entry_id hero_get_entry_id(void);
Entry *hero_get_entry(void);
bool hero_set_entry_id(entry_id loc_id);

#ifdef TEST
int hero_test_main(void);
#endif // TEST
#endif // HERO_H
