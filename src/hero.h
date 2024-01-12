#ifndef HERO_H
#define HERO_H

typedef struct Hero Hero;
Hero *hero_get_hero(void);

#ifdef TEST
int hero_test_main(void);
#endif // TEST
#endif // HERO_H
