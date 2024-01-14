#ifndef OUTPUT_H
#define OUTPUT_H
#include <string.h>
#include <stdarg.h> // varargs
#include <stdbool.h>
#include "string_utils.h"

// Declaration
int out(char *fmt, ...);
int out_flush(void);

// Definition
#ifdef OUTPUT_IMPLEMENTATION

#define OUTPUT_BUFFER_SZ 4096
static char output_buffer[OUTPUT_BUFFER_SZ];
#define SCRATCH_BUFFER_SZ 1024
static char temp_buffer[SCRATCH_BUFFER_SZ];

int out(char *fmt, ...) {    
    va_list args;
    va_start(args, fmt);
    // int vsnprintf (char * s, size_t n, const char * format, va_list arg);
    int num = vsnprintf(temp_buffer, SCRATCH_BUFFER_SZ, fmt, args);
    // perror("ERR: vsprintf failed in out()"); // TODO?

    // char * strncat ( char * destination, const char * source, size_t num );
    strncat(output_buffer, temp_buffer, size_to_fit_buffer(output_buffer, OUTPUT_BUFFER_SZ));
    va_end(args);
    return num;
}

int out_flush(void) {
    int num = printf("%s", output_buffer);
    output_buffer[0] = '\0';
    return num;
}

#endif // OUTPUT_IMPLEMENTATION
#endif // OUTPUT_H
