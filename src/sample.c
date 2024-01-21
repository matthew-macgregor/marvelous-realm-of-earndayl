#include "sample.h"

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "test_results.h"

static char *sample_test_baseline(void) {
    return 0;
}

static char *sample_test_all_tests(void) {
    mu_run_test(sample_test_baseline);

    // next test here
    return 0;
}

int sample_test_main(void) {
    const char *result = sample_test_all_tests();
    return report_test_results("Sample", result);
}
#endif // TEST
