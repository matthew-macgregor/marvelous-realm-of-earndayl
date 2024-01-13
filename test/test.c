#include <stdio.h>
#include <stdbool.h>

#include "minunit.h"
#define TEST_RESULTS_IMPLEMENTATION
#include "test_results.h"
#define STRING_UTILS_IMPLEMENTATION
#include "string_utils.h"
#include "input_parser.h"
#include "commands.h"
#include "colors.h"
#include "entry.h"
#include "hero.h"
#include "directions.h"

// extra_test.c
int extra_test_main(void);
// string_utils.h
// int string_utils_test_main();

int tests_run = 0;

static char* test_baseline(void) {
    mu_assert("error, true should be true", true == 1);
    return 0;
}

static char* test_interpret_command_baseline(void) {
    mu_assert("expected CMD_QUIT_I => false (quit)", interpret_command("quit") == false);
    return 0;
}

static char* all_tests(void) {
    mu_run_test(test_baseline);
    mu_run_test(test_interpret_command_baseline);
    // next test here

    return 0;
}

static int baseline_test_main(void) {
    char* result = all_tests();
    return report_test_results("Baseline", result);
}

int main(void) {
    int result = 0;
    result = baseline_test_main()
        || extra_test_main() 
        || parser_test_main()
        || string_utils_test_main()
        || entry_test_main()
        || hero_test_main()
        || directions_test_main();
    fprintf(stderr, CON_MAGENTA "Result: %s\n" CON_RESET, result == 0 ? "Ok" : "Failed");
    fprintf(stderr, CON_MAGENTA "Tests run: %d\n" CON_RESET, tests_run);
}
