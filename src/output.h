#ifndef OUTPUT_H
#define OUTPUT_H

// Defining our printf wrapper
#ifdef SILENT
#define printf_s(f_, ...)
#define puts(s)
#else
#define printf_s(f_, ...) printf((f_), __VA_ARGS__)
#define puts(s) printf("%s\n", s)
#endif

#endif
