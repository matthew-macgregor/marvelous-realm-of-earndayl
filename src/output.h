#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include "colors.h"

// Defining our printf wrapper
#ifdef SILENT

#define puts(f_, ...)
#define puts_ok(f_, ...)
#define puts_warn(f_, ...)
#define puts_err(f_, ...)

#else

#define puts(f_, ...) printf((f_ "\n"), __VA_ARGS__)
#define puts_ok(f_, ...) printf((CON_GREEN f_ CON_RESET "\n"), __VA_ARGS__)
#define puts_warn(f_, ...) printf((CON_YELLOW f_ CON_RESET "\n"), __VA_ARGS__)
#define puts_err(f_, ...) printf((CON_RED f_ CON_RESET "\n"), __VA_ARGS__)

#endif

#endif
