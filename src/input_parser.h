#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "commands.h"

struct CapturedPhraseResult {
    int captured_phrase_count;
    int placeholder_count;
};

extern COMMAND_ID parse_raw_input(char *input);
extern char *get_captured_phrase(char buffer_id);
extern int get_captured_phrase_count();

#ifdef TEST
extern int parser_test_main();
#endif // end TEST
#endif // end INPUT_PARSER_H