#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"

int tests_run = 0;

static char * test_baseline() {
    mu_assert("error, true should be true", true == 1);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_baseline);
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
