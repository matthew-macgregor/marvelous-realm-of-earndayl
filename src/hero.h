#ifndef HERO_H
#define HERO_H

#include "entry.h"

typedef struct Hero Hero;
Hero *hero_get_hero(void);
location_id hero_get_location(void);
bool hero_set_location(location_id loc_id);

#ifdef TEST
int hero_test_main(void);
#endif // TEST
#endif // HERO_H
