#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "input_parser.h"
#include "keywords.h"

/**
 * In a command pattern such as 'put A in B', the placeholders are A and B.
 * A phrase is captured for each pattern: 'put gold in wooden box', 'gold' and 'box' are captured.
 */

#define PARSER_CAPTURE_BUFFER_COUNT 26
#define PARSER_CAPTURE_BUFFER_SZ 100

static char parser_capture_buffer[PARSER_CAPTURE_BUFFER_COUNT][PARSER_CAPTURE_BUFFER_SZ];

#define GET_CAPTURE_BUFFER_BY_LETTER(letter) (parser_capture_buffer[(letter) - 'A'])
#define SIZE_TO_FIT_BUFFER(str, buffer_sz) ((buffer_sz) - strlen(str) - 1)
#define IS_CHAR_IN_RANGE(c) ((c) >= 'A' && (c) <= 'Z')

char *get_captured_phrase(char buffer_id) {
    return GET_CAPTURE_BUFFER_BY_LETTER(buffer_id);
}

int get_captured_phrase_count() {
    int count = 0;
    for (char c = 'A'; c <= 'Z'; c++) {
        char *buff = GET_CAPTURE_BUFFER_BY_LETTER(c);
        if (*buff == '\0') {
            break;
        }
        count++;
    }
    return count;
}

// Forward declaration
static struct CapturedPhraseResult parse_pattern(const char *input, const char *pattern);

COMMAND_ID parse_raw_input(char *input) {
    char *verb = strtok(input, " \n");
    // char *noun = strtok(NULL, " \n");

    parse_pattern(input, ""); // TODO: make it work

    if (verb != NULL)
    {
        if (strcmp(verb, CMD_QUIT_S) == 0) {
            return CMD_QUIT_I;
        }
        else if (strcmp(verb, CMD_LOOK_S) == 0) {
            return CMD_LOOK_I;
        }
    }

    return CMD_UNKNOWN_I;
}

static void clear_pattern_buffers() {
    for (char c = 'A'; c <= 'Z'; c++) {
        char *buffer = GET_CAPTURE_BUFFER_BY_LETTER(c);
        buffer[0] = '\0';
    }
}

/**
 * string: input to append to the buffer.
 * buffer_id: character identifier of the buffer (A-Z).
 */
static char* append_to_pattern_buffer(const char *input, const char buffer_id) {
    char *placeholder_buffer = GET_CAPTURE_BUFFER_BY_LETTER(buffer_id);
    if (strlen(placeholder_buffer) > 0) {
        // Add space between words
        strncat(placeholder_buffer, " ", SIZE_TO_FIT_BUFFER(placeholder_buffer, PARSER_CAPTURE_BUFFER_SZ));
    }
    return strncat(placeholder_buffer, input, SIZE_TO_FIT_BUFFER(placeholder_buffer, PARSER_CAPTURE_BUFFER_SZ));
}

/**
 * placeholder_id is a character in the range (A-Z).
 * token is one term in the phrase replacing the placeholder.
 * Returns a pointer to the placeholder params buffer if successful, or NULL if not.
 */
static char* try_consume_token(const char *token, const char placeholder_id) {
    if (token == NULL) {
        return NULL;
    }

    if (is_keyword(token)) {
        return NULL;
    }

    if (IS_CHAR_IN_RANGE(placeholder_id)) {
        char *param = GET_CAPTURE_BUFFER_BY_LETTER(placeholder_id);
        append_to_pattern_buffer(token, placeholder_id);
        return param;
    }

    return NULL;
}

#define TOKENS_DONT_MATCH(t1, t2) (((t1) && (t2) && (strcmp(t1, t2) != 0)) || ((t1) == NULL || (t2) == NULL))
#define TOKENS_MATCH(t1, t2) ((t1) && (t2) && (strcmp(t1, t2) == 0))
#define LOOKS_LIKE_PLACEHOLDER(tok) ((tok) && (strlen(tok) == 1) && isupper(tok[0]) && IS_CHAR_IN_RANGE(tok[0]))

/**
 * Given a string as input ("get golden axe") and a pattern ("get A"),
 * parse_pattern parses and consumes the non-keyword string for the 
 * pattern and stores it in one of 26 (A-Z) buffers. Buffers are checked
 * for overflow and will truncate text.
 * 
 * Returns the number of patterns that were matched.
 */
static struct CapturedPhraseResult parse_pattern(const char *input, const char *pattern) {
    clear_pattern_buffers();
    char *saveptr_input, *saveptr_pattern;

    char *input_copy = strdup(input);
    char *pattern_copy = strdup(pattern);

    char *input_tok = strtok_r(input_copy, " \n", &saveptr_input);
    char *pattern_tok = strtok_r(pattern_copy, " \n", &saveptr_pattern);

    struct CapturedPhraseResult result;
    result.captured_phrase_count = 0;
    result.placeholder_count = 0;

    while (input_tok != NULL || pattern_tok != NULL) {
        if (TOKENS_DONT_MATCH(input_tok, pattern_tok)) {
            // the tokens didn't match, take a closer look
            if (LOOKS_LIKE_PLACEHOLDER(pattern_tok)) {
                result.placeholder_count++;
                // this looks like it's a placeholder, so let's try to consume it
                bool did_match_at_least_one = false;
                while(try_consume_token(input_tok, *pattern_tok) != NULL) {
                    input_tok = strtok_r(NULL, " \n", &saveptr_input);
                    did_match_at_least_one = true;
                }

                if (did_match_at_least_one) result.captured_phrase_count++;
                pattern_tok = strtok_r(NULL, " \n", &saveptr_pattern);
                continue;
            }
        }

        // Next token
        input_tok = strtok_r(NULL, " \n", &saveptr_input);
        pattern_tok = strtok_r(NULL, " \n", &saveptr_pattern);
    }

    free(input_copy);
    free(pattern_copy);

    return result;
}

// TESTS
// ==================================================================
#ifdef TEST
#include "output.h"
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

extern int tests_run;

static char *test_TOKENS_DONT_MATCH() {
    mu_assert("TOKENS_DONT_MATCH: no match", TOKENS_DONT_MATCH("one", "two") == true);
    mu_assert("TOKENS_DONT_MATCH: match", TOKENS_DONT_MATCH("one", "one") == false);
    mu_assert("TOKENS_DONT_MATCH: no match space", TOKENS_DONT_MATCH("one", "one ") == true);
    mu_assert("TOKENS_DONT_MATCH: null(0)", TOKENS_DONT_MATCH(NULL, "one") == true);
    mu_assert("TOKENS_DONT_MATCH: null(1)", TOKENS_DONT_MATCH("one", NULL) == true);
    return 0;
}

static char *test_TOKENS_MATCH() {
    mu_assert("TOKENS_MATCH: no match", TOKENS_MATCH("one", "two") == false);
    mu_assert("TOKENS_MATCH: match", TOKENS_MATCH("one", "one") == true);
    mu_assert("TOKENS_MATCH: no match space", TOKENS_MATCH("one", "one ") == false);
    mu_assert("TOKENS_MATCH: null(0)", TOKENS_MATCH(NULL, "one") == false);
    mu_assert("TOKENS_MATCH: null(1)", TOKENS_MATCH("one", NULL) == false);
    return 0;
}

static char *test_GET_CAPTURE_BUFFER_BY_LETTER() {
    mu_assert("GET_CAPTURE_BUFFER_BY_LETTER 'A'", GET_CAPTURE_BUFFER_BY_LETTER('A') == parser_capture_buffer[0]);
    mu_assert("GET_CAPTURE_BUFFER_BY_LETTER 'B'", GET_CAPTURE_BUFFER_BY_LETTER('B') == parser_capture_buffer[1]);
    mu_assert("GET_CAPTURE_BUFFER_BY_LETTER 'Z'", GET_CAPTURE_BUFFER_BY_LETTER('Z') == parser_capture_buffer[25]);
    return 0;
}

static char *test_append_to_pattern_buffer() {
    clear_pattern_buffers();
    char *buffer = append_to_pattern_buffer("goblin", 'A');
    mu_assert("append_to_pattern_buffer", buffer == GET_CAPTURE_BUFFER_BY_LETTER('A'));
    mu_assert("append_to_pattern_buffer(test contents)", strcmp(buffer, "goblin") == 0);

    buffer = append_to_pattern_buffer("fangs", 'A');
    mu_assert("append_to_pattern_buffer", buffer == GET_CAPTURE_BUFFER_BY_LETTER('A'));
    mu_assert("append_to_pattern_buffer(append term)", strcmp(buffer, "goblin fangs") == 0);
    return 0;
}

static char *test_try_consume_token() {
    clear_pattern_buffers();
    try_consume_token("golden", 'A');
    try_consume_token("axe", 'A');
    mu_assert("try_consume_token('golden axe' => 'A')", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "golden axe") == 0);
    return 0;
}

static char *test_parse_pattern() {
    struct CapturedPhraseResult result = parse_pattern("get golden axe from orc", "get A from B");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern: golden axe", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "golden axe") == 0);
    mu_assert("parse_pattern: orc", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('B'), "orc") == 0);
    mu_assert("parse_pattern: get golden axe from orc count", get_captured_phrase_count() == 2);

    result = parse_pattern("put sparkling ruby into knapsack with smile", "put A into B with C");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count == 3);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 3);
    mu_assert("parse_pattern: sparkling ruby", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "sparkling ruby") == 0);
    mu_assert("parse_pattern: knapsack", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('B'), "knapsack") == 0);
    mu_assert("parse_pattern: smile", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('C'), "smile") == 0);
    mu_assert("parse_pattern: put sparkling ruby into knapsack with smile count", get_captured_phrase_count() == 3);

    result = parse_pattern("  get      golden axe    from    grim orc  ", "get A from B");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern: golden axe", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "golden axe") == 0);
    mu_assert("parse_pattern: grim orc", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('B'), "grim orc") == 0);
    mu_assert("parse_pattern: get golden axe from grim orc count", get_captured_phrase_count() == 2);

    result = parse_pattern("  get golden axe  from   green orc ", "  get   A from B  ");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern: golden axe", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "golden axe") == 0);
    mu_assert("parse_pattern: orc", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('B'), "green orc") == 0);
    mu_assert("parse_pattern: get golden axe from green orc", get_captured_phrase_count() == 2);
    return 0;
}

static char *test_parse_pattern_edge_cases() {
    struct CapturedPhraseResult result = parse_pattern("get golden axe from orc", "get A");
    mu_assert("parse_pattern edges: pattern is shorter, placeholder_count", result.placeholder_count == 1);
    mu_assert("parse_pattern edges: pattern is shorter, captured_phrase_count", result.captured_phrase_count == 1);
    mu_assert("parse_pattern edges: golden axe", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "golden axe") == 0);
    mu_assert("parse_pattern edges: empty", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('B'), "") == 0);
    mu_assert("parse_pattern edges: get golden axe from orc count", get_captured_phrase_count() == 1);

    result = parse_pattern("get golden axe", "get A from B");
    mu_assert("parse_pattern edges: pattern is longer, placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern edges: pattern is longer, captured_phrase_count", result.captured_phrase_count == 1);
    mu_assert("parse_pattern edges: golden axe", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('A'), "golden axe") == 0);
    mu_assert("parse_pattern edges: empty", strcmp(GET_CAPTURE_BUFFER_BY_LETTER('B'), "") == 0);
    mu_assert("parse_pattern edges: get golden axe count", get_captured_phrase_count() == 1);

    return 0;
}

static char *test_parse_pattern_mismatched() {
    struct CapturedPhraseResult result = parse_pattern("get golden axe", "get A from B");
    mu_assert("parse_pattern_mismatched_0", result.captured_phrase_count == 1 && result.placeholder_count == 2);

    return 0;
}

static char *test_parse_pattern_w_spaces() {
    char *input = "get   ";
    char *pattern = "get A";
    struct CapturedPhraseResult result = parse_pattern(input, pattern);
    mu_assert("expect false b/c pattern is missing placeholder, captured_phrase_count", result.captured_phrase_count == 0);
    mu_assert("expect false b/c pattern is missing placeholder, placeholder_count", result.placeholder_count == 1);

    input = "get golden axe from orc";
    pattern = "get A from B";
    result = parse_pattern(input, pattern);
    mu_assert("pattern and phrase match: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("pattern and phrase match: placeholder_count", result.placeholder_count == 2);
    return 0;
}

static char *test_pattern_buffer_overflow() {
    clear_pattern_buffers();
    for (size_t i = 0; i < (PARSER_CAPTURE_BUFFER_SZ / 5) + 50; i++) {
        append_to_pattern_buffer("sword", 'A');
    }

    mu_assert("overflow: string length", (PARSER_CAPTURE_BUFFER_SZ - 1) == strlen(GET_CAPTURE_BUFFER_BY_LETTER('A')));
    return 0;
}

static char *test_IS_CHAR_IN_RANGE() {
    mu_assert("expect true for 'A'", IS_CHAR_IN_RANGE('A') == true);
    mu_assert("expect true for 'Z'", IS_CHAR_IN_RANGE('Z') == true);
    mu_assert("expect false for 'a'", IS_CHAR_IN_RANGE('a') == false);
    mu_assert("expect false for 'z'", IS_CHAR_IN_RANGE('z') == false);
    mu_assert("expect false for ';'", IS_CHAR_IN_RANGE(';') == false);
    return 0;
}

static char *test_input_parser_baseline() {
    mu_assert("expected CMD_QUIT_I", parse_raw_input("quit") == CMD_QUIT_I);
    mu_assert("expected CMD_UNKNOWN_I", parse_raw_input("foo") == CMD_UNKNOWN_I);
    return 0;
}

static char *test_is_keyword() {
    mu_assert("expected keyword 'get' to be true", is_keyword("get") == true);
    mu_assert("expected keyword 'look' to be true", is_keyword("look") == true);
    mu_assert("expected keyword 'axe' to be false", is_keyword("axe") == false);
    mu_assert("expected keyword 'orc' to be false", is_keyword("orc") == false);
    return 0;
}

static char *test_looks_like_placeholder() {
    mu_assert("LOOKS_LIKE_PLACEHOLDER: expected true for A", LOOKS_LIKE_PLACEHOLDER("A") == true);
    mu_assert("LOOKS_LIKE_PLACEHOLDER: expected false for b", LOOKS_LIKE_PLACEHOLDER("b") == false);
    return 0;
}

static char *parser_test_all_tests() {
    mu_run_test(test_TOKENS_DONT_MATCH);
    mu_run_test(test_TOKENS_MATCH);
    mu_run_test(test_try_consume_token);
    mu_run_test(test_append_to_pattern_buffer);
    mu_run_test(test_parse_pattern);
    mu_run_test(test_parse_pattern_edge_cases);
    mu_run_test(test_parse_pattern_mismatched);
    mu_run_test(test_GET_CAPTURE_BUFFER_BY_LETTER);
    mu_run_test(test_input_parser_baseline);
    mu_run_test(test_parse_pattern_w_spaces);
    mu_run_test(test_IS_CHAR_IN_RANGE);
    mu_run_test(test_is_keyword);
    mu_run_test(test_looks_like_placeholder);
    mu_run_test(test_pattern_buffer_overflow);

    // next test here
    return 0;
}

int parser_test_main() {
    const char *result = parser_test_all_tests();
    return report_test_results("Parser", result);
}
#endif