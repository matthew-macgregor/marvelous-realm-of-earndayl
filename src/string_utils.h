#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <ctype.h>
#include <string.h>

#define STRIP_NEWLINE(s) input[strcspn(s, "\n")] = 0

// Declarations
bool is_empty_or_whitespace(const char *input);

// Implementation
#ifdef STRING_UTILS_IMPLEMENTATION
bool is_empty_or_whitespace(const char *input) {
    if (input == NULL) return true; //?
    if (*input == '\0') return true;
    for (char *str = (char *) input; *str != '\0'; str++) {
        if (!isblank(*str)) return false;
    }

    return true;
}
#endif // STRING_UTILS_IMPLEMENTATION

// TESTS
// ==================================================================
#ifdef TEST
#include "minunit.h"
#include "test_results.h"

extern int tests_run;

static char *test_is_empty_or_whitespace() {
    mu_assert("is empty", is_empty_or_whitespace(""));
    mu_assert("is empty with \0", is_empty_or_whitespace("\0"));
    mu_assert("is whitespace", is_empty_or_whitespace("     "));
    mu_assert("is not empty, trailing", is_empty_or_whitespace("$$$  ") == false);
    mu_assert("is not empty, leading", is_empty_or_whitespace("   $$$") == false);
    mu_assert("is not empty, leading + trailing", is_empty_or_whitespace("   $$$   ") == false);
    return 0;
}

static char *string_utils_test_all_tests() {
    mu_run_test(test_is_empty_or_whitespace);

    // next test here
    return 0;
}

int string_utils_test_main() {
    const char *result = string_utils_test_all_tests();
    return report_test_results("String Utils", result);
}

#endif // TEST
#endif // STRING_UTILS_H