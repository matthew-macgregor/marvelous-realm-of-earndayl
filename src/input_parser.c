#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "input_parser.h"
#include "string_utils.h"

/**
 * In a command pattern such as 'put A in B', the placeholders are A and B.
 * A phrase is captured for each pattern: 'put gold in wooden box', 'gold' and 'box' are captured.
 */

#define PARSER_CAPTURE_BUFFER_COUNT 26
#define PARSER_CAPTURE_BUFFER_SZ 100

static char parser_capture_buffer[PARSER_CAPTURE_BUFFER_COUNT][PARSER_CAPTURE_BUFFER_SZ];

#define get_capture_buffer_by_letter(letter) (parser_capture_buffer[(letter) - 'A'])

#define is_char_in_range(c) ((c) >= 'A' && (c) <= 'Z')

char *get_captured_phrase(char buffer_id) {
    return get_capture_buffer_by_letter(buffer_id);
}

/**
 * Returns the number of phrases captured from the input and pattern. 
 */
int get_captured_phrase_count(void) {
    int count = 0;
    for (char c = 'A'; c <= 'Z'; c++) {
        char *buff = get_capture_buffer_by_letter(c);
        if (*buff == '\0') {
            break;
        }
        count++;
    }
    return count;
}

static void clear_pattern_buffers(void) {
    for (char c = 'A'; c <= 'Z'; c++) {
        char *buffer = get_capture_buffer_by_letter(c);
        buffer[0] = '\0';
    }
}

/**
 * string: input to append to the buffer.
 * buffer_id: character identifier of the buffer (A-Z).
 */
static char* append_to_pattern_buffer(const char *input, const char buffer_id) {
    char *placeholder_buffer = get_capture_buffer_by_letter(buffer_id);
    if (strlen(placeholder_buffer) > 0) {
        // Add space between words
        strncat(placeholder_buffer, " ", size_to_fit_buffer(placeholder_buffer, PARSER_CAPTURE_BUFFER_SZ));
    }
    return strncat(placeholder_buffer, input, size_to_fit_buffer(placeholder_buffer, PARSER_CAPTURE_BUFFER_SZ));
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

    if (is_char_in_range(placeholder_id)) {
        char *param = get_capture_buffer_by_letter(placeholder_id);
        append_to_pattern_buffer(token, placeholder_id);
        return param;
    }

    return NULL;
}

#define tokens_dont_match(t1, t2) (((t1) && (t2) && (strcmp(t1, t2) != 0)) || ((t1) == NULL || (t2) == NULL))
#define tokens_match(t1, t2) ((t1) && (t2) && (strcmp(t1, t2) == 0))
#define looks_like_placeholder(tok) ((tok) && (strlen(tok) == 1) && is_char_in_range(tok[0]))

/**
 * Given a string as input ("get golden axe") and a pattern ("get A"),
 * parse_pattern parses and consumes the non-keyword string for the 
 * pattern and stores it in one of 26 (A-Z) buffers. Buffers are checked
 * for overflow and will truncate text.
 * 
 * Returns the number of patterns that were matched.
 */
extern CapturedPhraseResult parse_pattern(const char *input, const char *pattern) {
    clear_pattern_buffers();
    CapturedPhraseResult result = { 0, 0, false };
    if (input == NULL || pattern == NULL) return result;

    char *saveptr_input, *saveptr_pattern;

    char *input_copy = strdup(input);
    char *pattern_copy = strdup(pattern);

    char *input_tok = strtok_r(input_copy, " \n", &saveptr_input);
    char *pattern_tok = strtok_r(pattern_copy, " \n", &saveptr_pattern);


    result.captured_phrase_count = 0;
    result.placeholder_count = 0;
    result.matched = false;

    int matched_count = 0;
    int mismatched_count = 0;

    // Loop through each token from the input and the pattern. Every token should match, unless:
    // 
    // - a placeholder 'A' - 'Z' is encountered. In this case, consume the input matching the
    //   pattern, and stop when reaching a) end of string or b) the next token in the pattern.
    while (input_tok != NULL || pattern_tok != NULL) {
        if (tokens_dont_match(input_tok, pattern_tok)) {
            // the tokens didn't match, take a closer look
            if (looks_like_placeholder(pattern_tok)) {
                // we can look ahead to the next token in the pattern, and use it to know
                // when to terminate consuming the placeholder pattern.
                char *next_pattern_tok = strtok_r(NULL, " \n", &saveptr_pattern);
                result.placeholder_count++;

                bool did_match_at_least_one = false;
                while(
                    tokens_dont_match(input_tok, next_pattern_tok) && 
                    try_consume_token(input_tok, *pattern_tok) != NULL) {
                    // this looks like it's a placeholder, so let's try to consume it
                    input_tok = strtok_r(NULL, " \n", &saveptr_input);
                    did_match_at_least_one = true;
                }

                if (did_match_at_least_one) {
                    result.captured_phrase_count++;
                } else {
                    mismatched_count++;
                }
                pattern_tok = next_pattern_tok;
                continue;
            } else {
                mismatched_count++;
            }
        } else {
            // Tokens do match.
            matched_count++;
        }

        // Next token
        input_tok = strtok_r(NULL, " \n", &saveptr_input);
        pattern_tok = strtok_r(NULL, " \n", &saveptr_pattern);
    }

    free(input_copy);
    free(pattern_copy);

    result.matched = matched_count > 0 && mismatched_count == 0;
    return result;
}

// TESTS
// ==================================================================
#ifdef TEST
#include "colors.h"
#include "minunit.h"
#include "commands.h"
#include "test_results.h"

extern int tests_run;

static char *test_tokens_dont_match(void) {
    mu_assert("tokens_dont_match: no match", tokens_dont_match("one", "two") == true);
    mu_assert("tokens_dont_match: match", tokens_dont_match("one", "one") == false);
    mu_assert("tokens_dont_match: no match space", tokens_dont_match("one", "one ") == true);
    mu_assert("tokens_dont_match: null(0)", tokens_dont_match(NULL, "one") == true);
    mu_assert("tokens_dont_match: null(1)", tokens_dont_match("one", NULL) == true);
    return 0;
}

static char *test_TOKENS_MATCH(void) {
    mu_assert("tokens_match: no match", tokens_match("one", "two") == false);
    mu_assert("tokens_match: match", tokens_match("one", "one") == true);
    mu_assert("tokens_match: no match space", tokens_match("one", "one ") == false);
    mu_assert("tokens_match: null(0)", tokens_match(NULL, "one") == false);
    mu_assert("tokens_match: null(1)", tokens_match("one", NULL) == false);
    return 0;
}

static char *test_GET_CAPTURE_BUFFER_BY_LETTER(void) {
    mu_assert("get_capture_buffer_by_letter 'A'", get_capture_buffer_by_letter('A') == parser_capture_buffer[0]);
    mu_assert("get_capture_buffer_by_letter 'B'", get_capture_buffer_by_letter('B') == parser_capture_buffer[1]);
    mu_assert("get_capture_buffer_by_letter 'Z'", get_capture_buffer_by_letter('Z') == parser_capture_buffer[25]);
    return 0;
}

static char *test_append_to_pattern_buffer(void) {
    clear_pattern_buffers();
    char *buffer = append_to_pattern_buffer("goblin", 'A');
    mu_assert("append_to_pattern_buffer", buffer == get_capture_buffer_by_letter('A'));
    mu_assert("append_to_pattern_buffer(test contents)", strcmp(buffer, "goblin") == 0);

    buffer = append_to_pattern_buffer("fangs", 'A');
    mu_assert("append_to_pattern_buffer", buffer == get_capture_buffer_by_letter('A'));
    mu_assert("append_to_pattern_buffer(append term)", strcmp(buffer, "goblin fangs") == 0);
    return 0;
}

static char *test_try_consume_token(void) {
    clear_pattern_buffers();
    try_consume_token("golden", 'A');
    try_consume_token("axe", 'A');
    mu_assert("try_consume_token('golden axe' => 'A')", strcmp(get_capture_buffer_by_letter('A'), "golden axe") == 0);
    return 0;
}

static char *test_parse_pattern(void) {
    struct CapturedPhraseResult result = parse_pattern("get golden axe from orc", "get A from B");
    mu_assert("parse_pattern 1: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("parse_pattern 1: placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern 1: golden axe", strcmp(get_capture_buffer_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern 1: orc", strcmp(get_capture_buffer_by_letter('B'), "orc") == 0);
    mu_assert("parse_pattern 1: get golden axe from orc count", get_captured_phrase_count() == 2);

    result = parse_pattern("put sparkling ruby into knapsack with smile", "put A into B with C");
    mu_assert("parse_pattern 2: captured_phrase_count", result.captured_phrase_count == 3);
    mu_assert("parse_pattern 2: placeholder_count", result.placeholder_count == 3);
    mu_assert("parse_pattern 2: sparkling ruby", strcmp(get_capture_buffer_by_letter('A'), "sparkling ruby") == 0);
    mu_assert("parse_pattern 2: knapsack", strcmp(get_capture_buffer_by_letter('B'), "knapsack") == 0);
    mu_assert("parse_pattern 2: smile", strcmp(get_capture_buffer_by_letter('C'), "smile") == 0);
    mu_assert("parse_pattern 2: put sparkling ruby into knapsack with smile count", get_captured_phrase_count() == 3);

    result = parse_pattern("  get      golden axe    from    grim orc  ", "get A from B");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern: golden axe", strcmp(get_capture_buffer_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern: grim orc", strcmp(get_capture_buffer_by_letter('B'), "grim orc") == 0);
    mu_assert("parse_pattern: get golden axe from grim orc count", get_captured_phrase_count() == 2);

    result = parse_pattern("  get golden axe  from   green orc ", "  get   A from B  ");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count == 2);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern: golden axe", strcmp(get_capture_buffer_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern: orc", strcmp(get_capture_buffer_by_letter('B'), "green orc") == 0);
    mu_assert("parse_pattern: get golden axe from green orc", get_captured_phrase_count() == 2);

    result = parse_pattern("look in chest", "look in A");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count = 1);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 1);
    mu_assert("parse_pattern: chest", strcmp(get_capture_buffer_by_letter('A'), "chest") == 0);
    mu_assert("parse_pattern: look in chest", get_captured_phrase_count() == 1);

    result = parse_pattern("look in chest", "look at A");
    mu_assert("parse_pattern: captured_phrase_count", result.captured_phrase_count = 1);
    mu_assert("parse_pattern: placeholder_count", result.placeholder_count == 1);
    mu_assert("parse_pattern: matched", result.matched == false);
    mu_assert("parse_pattern: chest", strcmp(get_capture_buffer_by_letter('A'), "chest") == 0);
    mu_assert("parse_pattern: look at chest", get_captured_phrase_count() == 1);
    return 0;
}

static char *test_parse_pattern_edge_cases(void) {
    struct CapturedPhraseResult result = parse_pattern("get golden axe from orc", "get A");
    mu_assert("parse_pattern edges: pattern is shorter, placeholder_count", result.placeholder_count == 1);
    mu_assert("parse_pattern edges: pattern is shorter, captured_phrase_count", result.captured_phrase_count == 1);
    mu_assert("parse_pattern edges: golden axe", strcmp(get_capture_buffer_by_letter('A'), "golden axe from orc") == 0);
    mu_assert("parse_pattern edges: empty", strcmp(get_capture_buffer_by_letter('B'), "") == 0);
    mu_assert("parse_pattern edges: get golden axe from orc count", get_captured_phrase_count() == 1);

    result = parse_pattern("get golden axe", "get A from B");
    mu_assert("parse_pattern edges: pattern is longer, placeholder_count", result.placeholder_count == 2);
    mu_assert("parse_pattern edges: pattern is longer, captured_phrase_count", result.captured_phrase_count == 1);
    mu_assert("parse_pattern edges: golden axe", strcmp(get_capture_buffer_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern edges: empty", strcmp(get_capture_buffer_by_letter('B'), "") == 0);
    mu_assert("parse_pattern edges: get golden axe count", get_captured_phrase_count() == 1);

    return 0;
}

static char *test_parse_pattern_mismatched(void) {
    struct CapturedPhraseResult result = parse_pattern("get golden axe", "get A from B");
    mu_assert("parse_pattern_mismatched_0", result.captured_phrase_count == 1 && result.placeholder_count == 2);

    return 0;
}

static char *test_parse_pattern_w_spaces(void) {
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

static char *test_pattern_buffer_overflow(void) {
    clear_pattern_buffers();
    for (size_t i = 0; i < (PARSER_CAPTURE_BUFFER_SZ / 5) + 50; i++) {
        append_to_pattern_buffer("sword", 'A');
    }

    mu_assert("overflow: string length", (PARSER_CAPTURE_BUFFER_SZ - 1) == strlen(get_capture_buffer_by_letter('A')));
    return 0;
}

static char *test_is_char_in_range(void) {
    mu_assert("expect true for 'A'", is_char_in_range('A') == true);
    mu_assert("expect true for 'Z'", is_char_in_range('Z') == true);
    mu_assert("expect false for 'a'", is_char_in_range('a') == false);
    mu_assert("expect false for 'z'", is_char_in_range('z') == false);
    mu_assert("expect false for ';'", is_char_in_range(';') == false);
    return 0;
}

static char *test_input_parser_baseline(void) {
    mu_assert("expected quit", parse_pattern("quit", "quit").matched == true);
    mu_assert("expected quit: false", parse_pattern("foo", "quit").matched == false);
    return 0;
}

static char *test_looks_like_placeholder(void) {
    mu_assert("looks_like_placeholder: expected true for A", looks_like_placeholder("A") == true);
    mu_assert("looks_like_placeholder: expected false for b", looks_like_placeholder("b") == false);
    return 0;
}

static char *parser_test_all_tests(void) {
    mu_run_test(test_tokens_dont_match);
    mu_run_test(test_TOKENS_MATCH);
    mu_run_test(test_try_consume_token);
    mu_run_test(test_append_to_pattern_buffer);
    mu_run_test(test_parse_pattern);
    mu_run_test(test_parse_pattern_edge_cases);
    mu_run_test(test_parse_pattern_mismatched);
    mu_run_test(test_GET_CAPTURE_BUFFER_BY_LETTER);
    mu_run_test(test_input_parser_baseline);
    mu_run_test(test_parse_pattern_w_spaces);
    mu_run_test(test_is_char_in_range);
    mu_run_test(test_looks_like_placeholder);
    mu_run_test(test_pattern_buffer_overflow);

    // next test here
    return 0;
}

int parser_test_main(void) {
    const char *result = parser_test_all_tests();
    return report_test_results("Parser", result);
}
#endif
