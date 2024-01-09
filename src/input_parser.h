#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "commands.h"

extern command_id parse_raw_input(char *input);

#ifdef TEST
extern int parser_test_main();
#endif // end TEST
#endif // end INPUT_PARSER_H