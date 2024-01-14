#ifndef HERO_H
#define HERO_H

#include "entry.h"

typedef struct Hero {
    size_t location;
} Hero;

Hero *hero_init(void);
Hero *hero_get_hero(void);
location_id hero_get_location_id(void);
bool hero_set_location_id(location_id loc_id);

#ifdef TEST
int hero_test_main(void);
#endif // TEST
#endif // HERO_H
