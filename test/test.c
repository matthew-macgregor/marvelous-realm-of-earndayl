#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"
#include "input_parser.h"
#include "commands.h"
#include "colors.h"


#define TEST_RESULTS_IMPLEMENTATION
#include "test_results.h"

// extra_test.c
int extra_test_main();

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
    return report_test_results("Baseline", result);
}

int main() {
    int result = 0;
    result = baseline_test_main()
        || extra_test_main() 
        || parser_test_main();
    printf(CON_MAGENTA "Result: %s\n" CON_RESET, result == 0 ? "Ok" : "Failed");
    printf(CON_MAGENTA "Tests run: %d" CON_RESET, tests_run);
}
