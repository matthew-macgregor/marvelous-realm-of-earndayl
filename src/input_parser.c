#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"
#include "input_parser.h"
#include "keywords.h"

/**
 * In a phrase such as 'put A in B', the placeholders are A and B.
 * TODO: maybe this implementation would be easier with something less clever.
 * TODO: Better naming?
 */
#define MAX_PARSER_PLACEHOLDER_PARAMS 26
#define MAX_PARSER_PLACEHOLDER_BUFFER_SZ 100
static char params[MAX_PARSER_PLACEHOLDER_PARAMS][MAX_PARSER_PLACEHOLDER_BUFFER_SZ];
#define param_by_letter(letter) (params[letter - 'A'])
#define size_to_fit_buffer(str, buffer_sz) (buffer_sz - strlen(str) - 1)
#define is_char_in_range(c) (c >= 'A' && c <= 'Z')

// Forward declaration
static int parse_pattern(const char *input, const char *pattern);

command_id parse_raw_input(char *input) {
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
        char *buffer = param_by_letter(c);
        buffer[0] = '\0';
    }
}

/**
 * string: input to append to the buffer.
 * buffer_id: character identifier of the buffer (A-Z).
 */
static char* append_to_pattern_buffer(const char *input, const char buffer_id) {
    char *placeholder_buffer = param_by_letter(buffer_id);
    if (strlen(placeholder_buffer) > 0) {
        // Add space between words
        strncat(placeholder_buffer, " ", size_to_fit_buffer(input, MAX_PARSER_PLACEHOLDER_BUFFER_SZ));
    }
    return strncat(placeholder_buffer, input, size_to_fit_buffer(input, MAX_PARSER_PLACEHOLDER_BUFFER_SZ));
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

    if (is_char_in_range(placeholder_id)) {
        char *param = param_by_letter(placeholder_id);
        append_to_pattern_buffer(token, placeholder_id);
        return param;
    }

    return NULL;
}

#define tokens_dont_match(t1, t2) (strcmp(t1, t2) != 0)
#define looks_like_placeholder(tok) (strlen(tok) == 1 && isupper(tok[0]) && is_char_in_range(tok[0]))

static int parse_pattern(const char *input, const char *pattern) {
    clear_pattern_buffers();
    char *saveptr_input, *saveptr_pattern;

    char *input_copy = strdup(input);
    char *pattern_copy = strdup(pattern);

    char *input_tok = strtok_r(input_copy, " \n", &saveptr_input);
    char *pattern_tok = strtok_r(pattern_copy, " \n", &saveptr_pattern);

    int placeholder_count = 0; // TODO: Need a better name for it

    while (input_tok != NULL && pattern_tok != NULL) {
        if (tokens_dont_match(input_tok, pattern_tok)) {
            // the tokens didn't match, take a closer look
            if (looks_like_placeholder(pattern_tok)) {
                // this looks like it's a placeholder, so let's try to consume it
                bool did_match_at_least_one = false;
                while(try_consume_token(input_tok, *pattern_tok) != NULL) {
                    input_tok = strtok_r(NULL, " \n", &saveptr_input);
                    did_match_at_least_one = true;
                }

                if (did_match_at_least_one) placeholder_count++;
                pattern_tok = strtok_r(NULL, " \n", &saveptr_pattern);
                continue;
            } else {
                // this looks like our tokens are just different, so the strings differ
                break;
            }
        }

        // Next token
        input_tok = strtok_r(NULL, " \n", &saveptr_input);
        pattern_tok = strtok_r(NULL, " \n", &saveptr_pattern);
    }

    free(input_copy);
    free(pattern_copy);

    return placeholder_count;
}

// TESTS
// ==================================================================
#ifdef TEST
#include "output.h"
#include "colors.h"
#include "minunit.h"
#include "commands.h"

extern int tests_run;

static char *test_param_by_letter() {
    mu_assert("param_by_letter 'A'", param_by_letter('A') == params[0]);
    mu_assert("param_by_letter 'B'", param_by_letter('B') == params[1]);
    mu_assert("param_by_letter 'Z'", param_by_letter('Z') == params[25]);
    return 0;
}

static char *test_append_to_pattern_buffer() {
    clear_pattern_buffers();
    char *buffer = append_to_pattern_buffer("goblin", 'A');
    mu_assert("append_to_pattern_buffer", buffer == param_by_letter('A'));
    mu_assert("append_to_pattern_buffer(test contents)", strcmp(buffer, "goblin") == 0);

    buffer = append_to_pattern_buffer("fangs", 'A');
    mu_assert("append_to_pattern_buffer", buffer == param_by_letter('A'));
    mu_assert("append_to_pattern_buffer(append term)", strcmp(buffer, "goblin fangs") == 0);
    return 0;
}

static char *test_try_consume_token() {
    clear_pattern_buffers();
    try_consume_token("golden", 'A');
    try_consume_token("axe", 'A');
    mu_assert("try_consume_token('golden axe' => 'A')", strcmp(param_by_letter('A'), "golden axe") == 0);
    return 0;
}

static char *test_parse_pattern() {
    mu_assert("parse_pattern", parse_pattern("get golden axe from orc", "get A from B") == 2);
    mu_assert("parse_pattern: golden axe", strcmp(param_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern: orc", strcmp(param_by_letter('B'), "orc") == 0);
    
    mu_assert("parse_pattern", parse_pattern("put sparkling ruby into knapsack with smile", "put A into B with C") == 3);
    mu_assert("parse_pattern: sparkling ruby", strcmp(param_by_letter('A'), "sparkling ruby") == 0);
    mu_assert("parse_pattern: knapsack", strcmp(param_by_letter('B'), "knapsack") == 0);
    mu_assert("parse_pattern: smile", strcmp(param_by_letter('C'), "smile") == 0);

    mu_assert("parse_pattern", parse_pattern("  get      golden axe    from    grim orc  ", "get A from B") == 2);
    mu_assert("parse_pattern: golden axe", strcmp(param_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern: grim orc", strcmp(param_by_letter('B'), "grim orc") == 0);

    mu_assert("parse_pattern", parse_pattern("  get golden axe  from   green orc ", "  get   A from B  ") == 2);
    mu_assert("parse_pattern: golden axe", strcmp(param_by_letter('A'), "golden axe") == 0);
    mu_assert("parse_pattern: orc", strcmp(param_by_letter('B'), "green orc") == 0);

    return 0;
}

static char *test_parse_pattern_w_spaces() {
    char *input = "get   ";
    char *pattern = "get A";
    mu_assert("expect false b/c pattern is missing placeholder", parse_pattern(input, pattern) == 0);

    input = "get golden axe from orc";
    pattern = "get A from B";
    mu_assert("single placeholder: expect true", parse_pattern(input, pattern) == 2);
    return 0;
}

static char *test_is_char_in_range() {
    mu_assert("expect true for 'A'", is_char_in_range('A') == true);
    mu_assert("expect true for 'Z'", is_char_in_range('Z') == true);
    mu_assert("expect false for 'a'", is_char_in_range('a') == false);
    mu_assert("expect false for 'z'", is_char_in_range('z') == false);
    mu_assert("expect false for ';'", is_char_in_range(';') == false);
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

static char *parser_test_all_tests() {
    mu_run_test(test_try_consume_token);
    mu_run_test(test_append_to_pattern_buffer);
    mu_run_test(test_parse_pattern);
    mu_run_test(test_param_by_letter);
    mu_run_test(test_input_parser_baseline);
    mu_run_test(test_parse_pattern_w_spaces);
    mu_run_test(test_is_char_in_range);
    mu_run_test(test_is_keyword);

    // next test here
    return 0;
}

int parser_test_main() {
    const char *result = parser_test_all_tests();
    if (result != 0) {
        printf("%sParser: %s\n%s", CON_RED, result, CON_RESET);
    } else {
        printf("%sParser: TESTS PASSED\n%s", CON_GREEN, CON_RESET);
    }

    return result != 0;
}
#endif