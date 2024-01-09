#ifndef OUTPUT_H
#define OUTPUT_H

// Defining our printf wrapper
#ifdef SILENT

#define printf_s(f_, ...)
#define puts(s)
#define puts_ok(s)
#define puts_warn(s)
#define puts_err(s)

#else

#include <stdio.h>
#include "colors.h"

#define printf_s(f_, ...) printf((f_), __VA_ARGS__)
#define puts(s) printf("%s\n", s)
#define puts_ok(s) printf("%s%s\n%s", CON_GREEN, s, CON_RESET);
#define puts_warn(s) printf("%s%s\n%s", CON_YELLOW, s, CON_RESET);
#define puts_err(s) printf("%s%s\n%s", CON_RED, s, CON_RESET);

#endif

#endif
