#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "commands.h"

typedef struct CapturedPhraseResult {
    int captured_phrase_count;
    int placeholder_count;
    bool matched;
} CapturedPhraseResult;

extern CapturedPhraseResult parse_pattern(const char *input, const char *pattern);
extern char *parse_get_captured_phrase(char buffer_id);
extern int parse_get_captured_phrase_count(void);

#ifdef TEST
extern int parser_test_main(void);
#endif // end TEST
#endif // end INPUT_PARSER_H
