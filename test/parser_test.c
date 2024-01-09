#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"
#include "input_parser.h"
#include "commands.h"

extern int tests_run;

static char* test_param_by_letter() {
    mu_assert("param_by_letter 'A'", parser_placeholder_param_by_letter('A') == &parser_placeholder_params[0]);
    return 0;
}

static char* test_input_parser_baseline() {
    mu_assert("expected CMD_QUIT_I", parse_raw_input("quit") == CMD_QUIT_I);
    mu_assert("expected CMD_UNKNOWN_I", parse_raw_input("foo") == CMD_UNKNOWN_I);
    return 0;
}

static char* parser_test_all_tests() {
    mu_run_test(test_param_by_letter);
    mu_run_test(test_input_parser_baseline);
    // next test here
    return 0;
}

int parser_test_main() {
    char *result = parser_test_all_tests();
    if (result != 0) {
        printf("FAILED: %s\n", result);
    } else {
        printf("Parser: ALL TESTS PASSED\n");
    }

    return result != 0;
}
