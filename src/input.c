#include <strings.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "linenoise.h"
#include "string_utils.h"
#include "input_parser.h"
#include "commands.h"

//static void completion(const char *buf, linenoiseCompletions *lc) {
//	if (buf[0] == 'h') {
//		linenoiseAddCompletion(lc, "hello");
//		linenoiseAddCompletion(lc, "hello there");
//	}
//}

//static char *hints(const char *buf, int *color, int *bold) {
//	if (!strcasecmp(buf,"hello")) {
//		*color = 35;
//		*bold = 0;
//		return " World";
//	}
//	return NULL;
//}

void input_loop() {
//	TODO: enable completion and hints
//	linenoiseSetCompletionCallback(completion);
//	linenoiseSetHintsCallback(hints);
	linenoiseHistorySetMaxLen(20);

	char *input = NULL;
	bool result = true;

	while((input = linenoise(":> ")) != NULL) {
		s_trim_newline(input);
		if (is_empty_or_whitespace(input)) {
			linenoiseFree(input);
			continue;
		}

		if ((result = interpret_command(input))) linenoiseHistoryAdd(input);
		printf("\n");
		free(input);
		if (!result) break;
	}
}
