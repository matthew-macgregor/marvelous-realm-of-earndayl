#ifndef TEST_RESULTS_H
#define TEST_RESULTS_H

#include <stdio.h>
#include "colors.h"

/* Declaration */
int report_test_results(const char *suite, const char *result);

/* Implementation */
#ifdef TEST_RESULTS_IMPLEMENTATION
int report_test_results(const char *suite, const char *result) {
    if (result != 0) {
        printf(CON_RED "%s: %s\n" CON_RESET, suite, result);
    } else {
        printf(CON_GREEN "%s: TESTS PASSED" CON_RESET "\n",  suite);
    }

    return result != 0;
}

#endif // TEST_RESULTS_IMPLEMENTATION
#endif // TEST_RESULTS_H