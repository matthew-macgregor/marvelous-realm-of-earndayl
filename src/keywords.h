#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <string.h>

typedef struct {
    char *keyword;
} KEYWORD;

KEYWORD game_keywords[] = {
    { "look" },
    { "quit" },
    { "get" },
    { "in" },
    { "into" },
    { "from" },
    { "with" }
};

#define keyword_count  (sizeof game_keywords / sizeof *game_keywords)

bool is_keyword(const char *maybe_keyword) {
    KEYWORD k;
    for (unsigned long i = 0; i < keyword_count; ++i) {
        k = game_keywords[i];
        if (strcmp(k.keyword, maybe_keyword) == 0) {
            return true;
        }
    }

    return false;
}

#endif