#ifndef DICE_H
#define DICE_H

typedef struct DiceRoll {
   int min;
   int max;
   int mod;
   unsigned int rolls;
} DiceRoll;

#define dice_new_die(_min, _max) { (_min), (_max), 0, 1 }

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
