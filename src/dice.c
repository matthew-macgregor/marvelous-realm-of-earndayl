#include <time.h>
#include <stdlib.h>
#include "dice.h"

// srand(time(NULL));   // Initialization, should only be called once.
DiceRoll dice[] = {
    dice_new_die(1, 2),         // 1d2
    dice_new_die(1, 4),         // 1d4
    dice_new_die(1, 6),         // 1d6
    dice_new_die(1, 8),         // 1d8
    dice_new_die(1, 10),        // 1d10
    dice_new_die(1, 12),        // 1d12
    dice_new_die(1, 20),        // 1d20
    dice_new_die(1, 100),       // 1d100
    { 1, 6, 0, 3 }              // 3d6
};

int dice_roll(DiceRoll *range) {
    int result = 0;
    for (uint8_t i = 0; i < range->rolls; i++) {
        result += rand() % (range->max + 1 - range->min) + range->min;
    }
    result += range->mod;
    return result;
}

int dice_roll_1d4(void) {
    return dice_roll(d1d4);
}

int dice_roll_1d6(void) {
    return dice_roll(d1d6);
}

int dice_roll_3d6(void) {
    return dice_roll(d3d6);
}

int dice_roll_1d8(void) {
    return dice_roll(d1d8);
}

int dice_roll_1d12(void) {
    return dice_roll(d1d12);
}

int dice_roll_1d20(void) {
    return dice_roll(d1d20);
}

int dice_roll_1d100(void) {
    return dice_roll(d1d100);
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "test_results.h"

static char *dice_test_baseline(void) {
    int roll = dice_roll(d1d4);
    mu_assert("1d4", roll > 0 && roll <= 4);
    roll = dice_roll(d1d6);
    mu_assert("1d6", roll > 0 && roll <= 6);
    roll = dice_roll(d1d8);
    mu_assert("1d8", roll > 0 && roll <= 8);
    roll = dice_roll(d1d10);
    mu_assert("1d10", roll > 0 && roll <= 10);
    roll = dice_roll(d1d12);
    mu_assert("1d12", roll > 0 && roll <= 12);
    roll = dice_roll(d1d20);
    mu_assert("1d20", roll > 0 && roll <= 20);
    roll = dice_roll(d1d100);
    mu_assert("1d100", roll > 0 && roll <= 100);
    return 0;
}

static char *dice_test_all_tests(void) {
    // srand(123);
    mu_run_test(dice_test_baseline);

    // next test here
    return 0;
}

int dice_test_main(void) {
    const char *result = dice_test_all_tests();
    return report_test_results("Dice", result);
}
#endif // TEST
