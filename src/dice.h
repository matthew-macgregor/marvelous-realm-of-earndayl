#ifndef DICE_H
#define DICE_H

typedef struct DiceRoll {
   int min;
   int max;
   int mod;
   unsigned int rolls;
} DiceRoll;

#define dice_new_die(_min, _max) { (_min), (_max), 0, 1 }
extern DiceRoll dice[];
#define d1d2        (dice + 0)
#define d1d4        (dice + 1)
#define d1d6        (dice + 2)
#define d1d8        (dice + 3)
#define d1d10       (dice + 4)
#define d1d12       (dice + 5)
#define d1d20       (dice + 6)
#define d1d100      (dice + 7)
#define d3d6        (dice + 8)

int dice_roll_1d4(void);
int dice_roll_1d6(void);
int dice_roll_1d8(void);
int dice_roll_1d12(void);
int dice_roll_1d20(void);
int dice_roll_1d100(void);

#ifdef TEST
int dice_test_main(void);
#endif // TEST
#endif // DICE_H
