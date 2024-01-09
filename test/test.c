#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"
#include "input_parser.h"
#include "commands.h"
#include "colors.h"

// parser_test.c
int parser_test_main();

int tests_run = 0;

static char* test_baseline() {
    mu_assert("error, true should be true", true == 1);
    return 0;
}



static char* test_interpret_command_baseline() {
    mu_assert("expected CMD_QUIT_I => false (quit)", interpret_command(CMD_QUIT_I) == false);
    mu_assert("expected CMD_UNKNOWN_I => true (continue)", interpret_command(CMD_UNKNOWN_I));
    return 0;
}

static char* all_tests() {
    mu_run_test(test_baseline);
    mu_run_test(test_interpret_command_baseline);
    // next test here

    return 0;
}

static int baseline_test_main() {
    char* result = all_tests();
    if (result != 0) {
        printf("%sBaseline: %s\n%s", CON_RED, result, CON_RESET);
    } else {
        printf("%sBaseline: TESTS PASSED\n%s", CON_GREEN, CON_RESET);
    }

    return result != 0;
}

int main() {
    int result = 0;
    result = parser_test_main();
    result = baseline_test_main() || result;
    printf("Tests run: %d\n", tests_run);
}
