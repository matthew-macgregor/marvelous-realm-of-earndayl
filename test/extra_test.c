#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"
#include "input_parser.h"
#include "commands.h"
#include "colors.h"
#include "test_results.h"

extern int tests_run;

static char* extra_test_all_tests(void) {
    // next test here
    return 0;
}

int extra_test_main(void) {
    char *result = extra_test_all_tests();
    return report_test_results("Extra", result);
}
