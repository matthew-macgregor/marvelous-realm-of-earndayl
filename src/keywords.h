#ifndef KEYWORDS_H
#define KEYWORDS_H

#include <string.h>

#define is_keyword(k) (strstr("from;get;in;into;look;quit;with", k) != NULL)

#endif