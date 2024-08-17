#ifndef COLORS_OFF
// Name            FG  BG
// ----------------------
// Black           30  40
#define CON_BLACK "\033[0;30m"
// Red             31  41
#define CON_RED "\033[0;31m"
// Green           32  42
#define CON_GREEN "\033[0;32m"
// Yellow          33  43
#define CON_YELLOW "\033[0;33m"
// Blue            34  44
#define CON_BLUE "\033[0;34m"
// Magenta         35  45
#define CON_MAGENTA "\033[0;35m"
// Cyan            36  46
#define CON_CYAN "\033[0;36m"
// White           37  47
// Bright Black    90  100
#define CON_BRIGHT_BLACK "\033[0;90m"
// Bright Red      91  101
// Bright Green    92  102
// Bright Yellow   93  103
// Bright Blue     94  104
// Bright Magenta  95  105
// Bright Cyan     96  106
// Bright White    97  107
#define CON_RESET "\033[0m"
#else
#define CON_BLACK ""
#define CON_RED ""
#define CON_GREEN ""
#define CON_YELLOW ""
#define CON_BLUE ""
#define CON_MAGENTA ""
#define CON_CYAN ""
#define CON_BRIGHT_BLACK ""
#define CON_RESET ""
#endif
