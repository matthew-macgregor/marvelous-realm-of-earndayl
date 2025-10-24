// #include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "constants.h"
#include "string_utils.h"
#include "directions.h"

#define DIRECTION_MAX_LENGTH 12

/* N  / E  / S  / W / NE  / NW  / SE /  SW */
const char *direction_name_mapping[] = {
    "north",
    "east",
    "south",
    "west",
    "northeast",
    "northwest",
    "southeast",
    "southwest"
};

/*  N  / E  / S  / W    NE  / NW  / SE /  SW */
static char* direction_letter_mapping[8] = { "N", "E", "S", "W", "NE", "NW", "SE", "SW" };
static char* direction_letter_mapping_lower[8] = { "n", "e", "s", "w", "ne", "nw", "se", "sw" };
#define DIRECTION_NAME_MAPPING_COUNT (sizeof direction_name_mapping / sizeof *direction_name_mapping)
#define DIRECTION_LETTER_MAPPING_COUNT (sizeof direction_letter_mapping / sizeof *direction_letter_mapping)
#define is_direction_abbr(s) (strlen((s)) < 4)

/**
 * Given a Direction, returns a constant string to the text representation.
 * Example: NORTH -> "north"
 */
extern const char* direction_to_text(const Direction d) {
    if (d == DIRECTION_UNKNOWN) {
        return "unknown";
    }

    return direction_name_mapping[d];
}

/**
 * Given a Direction, returns a constant string to the abbreviated version.
 * Example: NORTH -> "N"
 */
extern const char* direction_to_abbr(const Direction d) {
    if (d == DIRECTION_UNKNOWN) {
        return "U";
    }

    return direction_letter_mapping[d];
}

/**
 * Given a string direction, normalizes it into the expected format.
 */
static const char* normalize_direction_string(const char *direction) {
    static char buffer[DIRECTION_MAX_LENGTH + 1];
    int buff_idx = 0;
    for (size_t i = 0; i < strlen(direction) && i < DIRECTION_MAX_LENGTH; i++) {
        char c = tolower(direction[i]);
        if (!isspace(c)) {
            buffer[buff_idx++] = c;
        }
    }
    buffer[buff_idx] = '\0';

    return buffer;
}

/**
 * Given a phrase ("west", "north  east"), returns a Direction.
 * Important: DIRECTION_UNKNOWN (-1) can be returned.
 */
extern Direction direction_text_to_direction(const char *direction) {
    const char *normalized = normalize_direction_string(direction);

    if (is_direction_abbr(normalized)) {
        for (int i = 0; i < (int)DIRECTION_LETTER_MAPPING_COUNT; i++) {
            const char *exit_key_name = direction_letter_mapping_lower[i];
            if (strcmp(exit_key_name, normalized) == 0) {
                return i;
            }
        }
    } else {
        for (int i = 0; i < (int)DIRECTION_LETTER_MAPPING_COUNT; i++) {
            const char *exit_key_name = direction_name_mapping[i];
            if (strcmp(exit_key_name, normalized) == 0) {
                return i;
            }
        }
    }

    return DIRECTION_UNKNOWN;
}

#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

static char *test_direction_to_abbr(void) {
    mu_assert("direction_to_abbr(NORTH)", strcmp(direction_to_abbr(NORTH), "N") == 0);
    mu_assert("direction_to_abbr(WEST)", strcmp(direction_to_abbr(WEST), "W") == 0);
    mu_assert("direction_to_abbr(SOUTH)", strcmp(direction_to_abbr(SOUTH), "S") == 0);
    mu_assert("direction_to_abbr(EAST)", strcmp(direction_to_abbr(EAST), "E") == 0);
    mu_assert("direction_to_abbr(NORTHEAST)", strcmp(direction_to_abbr(NORTHEAST), "NE") == 0);
    mu_assert("direction_to_abbr(EAST)", strcmp(direction_to_abbr(NORTHWEST), "NW") == 0);
    mu_assert("direction_to_abbr(SOUTHEAST)", strcmp(direction_to_abbr(SOUTHEAST), "SE") == 0);
    mu_assert("direction_to_abbr(SOUTHWEST)", strcmp(direction_to_abbr(SOUTHWEST), "SW") == 0);
    return 0;
}

static char *test_direction_to_text(void) {
    mu_assert("direction_to_text(NORTH)", strcmp(direction_to_text(NORTH), "north") == 0);
    mu_assert("direction_to_text(WEST)", strcmp(direction_to_text(WEST), "west") == 0);
    mu_assert("direction_to_text(SOUTH)", strcmp(direction_to_text(SOUTH), "south") == 0);
    mu_assert("direction_to_text(EAST)", strcmp(direction_to_text(EAST), "east") == 0);
    mu_assert("direction_to_text(NORTHEAST)", strcmp(direction_to_text(NORTHEAST), "northeast") == 0);
    mu_assert("direction_to_text(EAST)", strcmp(direction_to_text(NORTHWEST), "northwest") == 0);
    mu_assert("direction_to_text(SOUTHEAST)", strcmp(direction_to_text(SOUTHEAST), "southeast") == 0);
    mu_assert("direction_to_text(SOUTHWEST)", strcmp(direction_to_text(SOUTHWEST), "southwest") == 0);
    return 0;
}

static char *test_direction_name_to_direction(void) {
    char input[64] = { "west" };
    mu_assert("direction_text_to_direction: west", direction_text_to_direction(input) == WEST);

    memcpy(input, "w\0", 2);
    mu_assert("direction_text_to_direction: w", direction_text_to_direction(input) == WEST);

    memcpy(input, "east\0", 5);
    mu_assert("direction_text_to_direction: east", direction_text_to_direction(input) == EAST);

    memcpy(input, "north\0", 5);
    mu_assert("direction_text_to_direction: north", direction_text_to_direction(input) == NORTH);

    memcpy(input, "south\0", 5);
    mu_assert("direction_text_to_direction: south", direction_text_to_direction(input) == SOUTH);

    memcpy(input, "southeast\0", 10);
    mu_assert("direction_text_to_direction: southeast", direction_text_to_direction(input) == SOUTHEAST);

    memcpy(input, "south east\0", 11);
    mu_assert("direction_text_to_direction: south east", direction_text_to_direction(input) == SOUTHEAST);

    memcpy(input, "southwest\0", 10);
    mu_assert("direction_text_to_direction: southwest", direction_text_to_direction(input) == SOUTHWEST);

    memcpy(input, "south west\0", 11);
    mu_assert("direction_text_to_direction: south west", direction_text_to_direction(input) == SOUTHWEST);

    memcpy(input, "northeast\0", 10);
    mu_assert("direction_text_to_direction: northeast", direction_text_to_direction(input) == NORTHEAST);

    memcpy(input, "north east\0", 11);
    mu_assert("direction_text_to_direction: north east", direction_text_to_direction(input) == NORTHEAST);

    #define TEST_STR_0 "north   east\0"
    memcpy(input, TEST_STR_0, (sizeof TEST_STR_0 / sizeof 'c'));
    mu_assert("direction_text_to_direction: north   east", direction_text_to_direction(input) == NORTHEAST);

    #define TEST_STR_1 "foo\n"
    memcpy(input, TEST_STR_1, (sizeof TEST_STR_1 / sizeof 'c'));
    mu_assert("direction_text_to_direction: unknown   east", direction_text_to_direction(input) == DIRECTION_UNKNOWN);

    return 0;
}

static char *directions_test_all_tests(void) {
    mu_run_test(test_direction_name_to_direction);
    mu_run_test(test_direction_to_abbr);
    mu_run_test(test_direction_to_text);

    // next test here
    return 0;
}

int directions_test_main(void) {
    const char *result = directions_test_all_tests();
    return report_test_results("Directions", result);
}
#endif // TEST
