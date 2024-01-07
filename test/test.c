#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"
#include "input_parser.h"
#include "commands.h"


int tests_run = 0;

static char* test_baseline() {
    mu_assert("error, true should be true", true == 1);
    return 0;
}

static char* test_input_parser_baseline() {
    mu_assert("expected CMD_QUIT_I", parse_raw_input("quit") == CMD_QUIT_I);
    mu_assert("expected CMD_UNKNOWN_I", parse_raw_input("foo") == CMD_UNKNOWN_I);
    return 0;
}

static char* test_interpret_command_baseline() {
    mu_assert("expected CMD_QUIT_I => false (quit)", interpret_command(CMD_QUIT_I) == false);
    mu_assert("expected CMD_UNKNOWN_I => true (continue)", interpret_command(CMD_UNKNOWN_I));
    return 0;
}

static char* all_tests() {
    mu_run_test(test_baseline);
    mu_run_test(test_input_parser_baseline);
    mu_run_test(test_interpret_command_baseline);
    // next test here
    return 0;
}

// int main(int argc, char **argv) {
int main() {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
