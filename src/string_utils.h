#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string.h>

// Declarations
bool is_empty_or_whitespace(const char *input);
void s_strip_newline(char *input, size_t buffer_size);

// Implementation
#ifdef STRING_UTILS_IMPLEMENTATION
bool is_empty_or_whitespace(const char *input) {
    if (input == NULL) return true; //?
    if (*input == '\0') return true;
    for (char *str = (char *) input; *str != '\0'; str++) {
        if (!(*str == ' ' || *str == '\n' || *str == '\t')) return false;
    }

    return true;
}

void s_strip_newline(char *input, size_t buffer_size) {
    if (!input) return;
    for (size_t i = 0; input[i] != '\0' && i < buffer_size; i++) {
        if (input[i] == '\n') {
            input[i] = '\0';
            break;
        }
    }
}

#endif // STRING_UTILS_IMPLEMENTATION

// TESTS
// ==================================================================
#ifdef TEST
#include "minunit.h"
#include "test_results.h"

extern int tests_run;

static char *test_strip_newline(void) {
    char input[50];
    memcpy(input, "\n", 2);
    s_strip_newline(input, 50);
    mu_assert("empty", input[0] == '\0');

    memcpy(input, "aaaaa\n", 6);
    s_strip_newline(input, 50);
    mu_assert("whitespace", input[5] == '\0');
    return 0;
}

static char *test_is_empty_or_whitespace(void) {
    mu_assert("is empty", is_empty_or_whitespace(""));
    mu_assert("is empty with \0", is_empty_or_whitespace("\0"));
    mu_assert("is whitespace", is_empty_or_whitespace("     "));
    mu_assert("is not empty, trailing", is_empty_or_whitespace("$$$  ") == false);
    mu_assert("is not empty, leading", is_empty_or_whitespace("   $$$") == false);
    mu_assert("is not empty, leading + trailing", is_empty_or_whitespace("   $$$   ") == false);
    return 0;
}

static char *string_utils_test_all_tests(void) {
    mu_run_test(test_is_empty_or_whitespace);
    mu_run_test(test_strip_newline);

    // next test here
    return 0;
}

int string_utils_test_main(void) {
    const char *result = string_utils_test_all_tests();
    return report_test_results("String Utils", result);
}

#endif // TEST
#endif // STRING_UTILS_H
