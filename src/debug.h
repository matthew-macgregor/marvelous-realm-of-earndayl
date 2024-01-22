#ifndef DEBUG_H
#define DEBUG_H
#include "colors.h"

#ifdef DEBUG
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF
#endif

#endif
