
### JTN002 - MinUnit \-- a minimal unit testing framework for C

- by John Brewer
- *Content 01/07/2024 from [https://jera.com/techinfo/jtns/jtn002](https://jera.com/techinfo/jtns/jtn002)*

> LICENSE: You may use the code in this tech note for any purpose, with the understanding 
that it comes with NO WARRANTY.

### Introduction

Unit testing frameworks are quite popular in the object-oriented
programming world. Frameworks like JUnit (for Java), SUnit (for
Smalltalk), and CppUnit (for C++) provide a rich set of functionality.
However, this rich set of functionality can be intimidating to someone
who wants to do unit testing in a more constrained environment, such as
an embedded system written in C. But the important thing about unit
testing is the testing, not the framework. MinUnit is
an *extremely* simple unit testing framework written in C. It uses no
memory allocation, so it should work fine under almost any circumstance,
including ROMable code.

### Source Code

     /* file: minunit.h */
     #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
     #define mu_run_test(test) do { char *message = test(); tests_run++; \
                                    if (message) return message; } while (0)
     extern int tests_run;

No, that\'s not a typo. It\'s just 3 lines of code (4 here because I
wrapped a long line.)

### Setting Up A Test Case

A MinUnit test case is just a function that returns 0 (null) if the
tests pass. If the test fails, the function should return a string
describing the failing test. mu_assert is simply a macro that returns a
string if the expression passed to it is false. The mu_runtest macro
calls another test case and returns if that test case fails. That\'s all
there is to it! 

### Example

The following example runs two tests, one that passes, one that fails.

```c
/* file minunit_example.c */

#include <stdio.h>
#include "minunit.h"

int tests_run = 0;

int foo = 7;
int bar = 4;

static char * test_foo() {
    mu_assert("error, foo != 7", foo == 7);
    return 0;
}

static char * test_bar() {
    mu_assert("error, bar != 5", bar == 5);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_foo);
    mu_run_test(test_bar);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
```

### Conclusion

People think that writing a unit testing framework has to be complex. In
fact, you can write one in just a few lines of code, as this tech note
shows. Of course, if you have access to a full-featured testing
framework like JUnit, by all means use it. But if you don\'t, you can
still use a simple framework like MinUnit, or whip up your own in a few
hours.  *There\'s no excuse for not unit testing.*

### License

You may use the code in this tech note for any purpose, with the
understanding that it comes with NO WARRANTY.

### Appendix: Why do {} while?

I\'ve gotten a number of questions about the use of do {} while in the
mu_run_test macro. This is a standard C idiom for writing a macro that
contains multiple statements. For more information, see:

http://www.eskimo.com/~scs/C-faq/q10.4.html
