/*
**  tap4embedded : http://github.com/fperrad/tap4embedded
**
**  Copyright (C) 2016 Francois Perrad.
**
**  tap4embedded is free software; you can redistribute it and/or modify it
**  under the terms of the Artistic License 2.0
*/

#ifndef TAP_H
#define TAP_H

#ifndef TAP_PUTCHAR
#include <stdio.h>
#define TAP_PUTCHAR(c)          fputc((c), stdout)
#define TAP_FLUSH()             fflush(stdout)
#endif

#ifndef TAP_FLUSH
#define TAP_FLUSH()
#endif

#if defined(__unix__) || defined(_WIN32) || defined(_WIN64)
#include <stdlib.h>
#define TAP_EXIT(n)             exit(n)
#else
#define TAP_EXIT(n)
#endif

#if defined(__bool_true_false_are_defined) || defined(__cplusplus)
#define TAP_BOOL                bool
#define TAP_TRUE                true
#define TAP_FALSE               false
#else
#define TAP_BOOL                char
#define TAP_TRUE                1
#define TAP_FALSE               0
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void plan(unsigned int nb);
extern void no_plan(void);
extern void skip_all(const char *reason);
extern void done_testing(unsigned int nb);
extern void bail_out(const char *reason);
extern void ok(const char *file, unsigned int line, TAP_BOOL test, const char *name);
extern void todo(const char *reason, unsigned int count);
extern void skip(const char *reason, unsigned int count);
extern void todo_skip(const char *reason);
extern void skip_rest(const char *reason);
extern void diag (const char *msg);
extern int exit_status (void);

#define OK(test, name)          ok(__FILE__, __LINE__, (test), (name))
#define NOK(test, name)         ok(__FILE__, __LINE__, !(test), (name))
#define PASS(name)              ok(__FILE__, __LINE__, TAP_TRUE, (name))
#define FAIL(name)              ok(__FILE__, __LINE__, TAP_FALSE, (name))

#ifndef TAP_NO_IMPL

typedef struct {
    unsigned int curr_test;
    unsigned int expected_tests;
    unsigned int todo_upto;
    const char *todo_reason;
    TAP_BOOL have_plan;
    TAP_BOOL no_plan;
    TAP_BOOL have_output_plan;
    TAP_BOOL done_testing;
    TAP_BOOL is_passing;
} tap_t;

tap_t tap;

void putstr(const char *msg) {
    TAP_BOOL need_begin = TAP_FALSE;
    for (; *msg != '\0'; msg++) {
        if (need_begin) {
            TAP_PUTCHAR('#');
            TAP_PUTCHAR(' ');
            need_begin = TAP_FALSE;
        }
        TAP_PUTCHAR(*msg);
        if (*msg == '\n') {
            need_begin = TAP_TRUE;
        }
    }
}

void putuint (unsigned int n) {
    char buf[8 * sizeof(unsigned int) + 1];
    char *ptr = &buf[sizeof(buf) - 1];
    *ptr = '\0';
    do {
        char c = '0' + (n % 10u);
        --ptr;
        *ptr = c;
        n /= 10u;
    } while (n != 0u);
    putstr(ptr);
}

void not_yet_plan(void) {
    if (tap.have_plan) {
        putstr("You tried to plan twice\n");
        TAP_FLUSH();
        TAP_EXIT(-1);
    }
}

void plan(unsigned int nb) {
    not_yet_plan();
    putstr("1..");
    putuint(nb);
    TAP_PUTCHAR('\n');
    tap.expected_tests = nb;
    tap.have_plan = TAP_TRUE;
    tap.have_output_plan = TAP_TRUE;
    tap.is_passing = TAP_TRUE;
}

void no_plan(void) {
    not_yet_plan();
    tap.have_plan = TAP_TRUE;
    tap.no_plan = TAP_TRUE;
    tap.is_passing = TAP_TRUE;
}

void skip_all(const char *reason) {
    not_yet_plan();
    putstr("1..0 # SKIP ");
    putstr(reason);
    TAP_PUTCHAR('\n');
    tap.have_output_plan = TAP_TRUE;
    TAP_FLUSH();
}

void done_testing(unsigned int nb) {
    if (tap.done_testing) {
        putstr("done_testing() was already called\n");
    }
    else {
        tap.done_testing = TAP_TRUE;
        if ((tap.expected_tests != nb) && (tap.expected_tests != 0u)) {
            putstr("# Looks like you planned ");
            putuint(tap.expected_tests);
            putstr(" tests but ran ");
            putuint(nb);
            TAP_PUTCHAR('.');
            TAP_PUTCHAR('\n');
        }
        else {
            tap.expected_tests = nb;
        }
        if (! tap.have_output_plan) {
            putstr("1..");
            putuint(nb);
            TAP_PUTCHAR('\n');
            tap.have_output_plan = TAP_TRUE;
        }
        if ((tap.expected_tests != tap.curr_test) || (tap.curr_test == 0u)) {
            tap.is_passing = TAP_FALSE;
        }
        putstr("# Done with tap4embedded.\n");
    }
    TAP_FLUSH();
}

void bail_out(const char *reason) {
    putstr("Bail out!");
    if (reason != NULL) {
        TAP_PUTCHAR(' ');
        TAP_PUTCHAR(' ');
        putstr(reason);
    }
    TAP_PUTCHAR('\n');
    TAP_FLUSH();
}

void need_plan(void) {
    if (! tap.have_plan) {
        putstr("You tried to run a test without a plan\n");
        TAP_FLUSH();
        TAP_EXIT(-1);
    }
}

void ok(const char *file, unsigned int line, TAP_BOOL test, const char *name) {
    need_plan();
    ++tap.curr_test;
    if (! test) {
        putstr("not ");
    }
    putstr("ok ");
    putuint(tap.curr_test);
    if (name != NULL) {
        putstr(" - ");
        putstr(name);
    }
    if ((tap.todo_reason != NULL) && (tap.todo_upto >= tap.curr_test)) {
        putstr(" # TODO # ");
        putstr(tap.todo_reason);
    }
    TAP_PUTCHAR('\n');
    if (! test) {
        putstr("#    Failed");
        if (tap.todo_upto >= tap.curr_test) {
            putstr(" (TODO)");
        }
        else {
            tap.is_passing = TAP_FALSE;
        }
        putstr(" test (");
        putstr(file);
        putstr(" at line ");
        putuint(line);
        putstr(")\n");
    }
}

void todo(const char *reason, unsigned int count) {
    tap.todo_upto = tap.curr_test + count;
    tap.todo_reason = reason;
}

void skip(const char *reason, unsigned int count) {
    unsigned int i;
    need_plan();
    for (i = 0u; i < count; i++) {
        ++tap.curr_test;
        putstr("ok ");
        putuint(tap.curr_test);
        putstr(" - # skip");
        if (reason != NULL) {
            TAP_PUTCHAR(' ');
            putstr(reason);
        }
        TAP_PUTCHAR('\n');
    }
}

void todo_skip(const char *reason) {
    need_plan();
    ++tap.curr_test;
    putstr("not ok ");
    putuint(tap.curr_test);
    putstr(" - # TODO & SKIP");
    if (reason != NULL) {
        TAP_PUTCHAR(' ');
        putstr(reason);
    }
    TAP_PUTCHAR('\n');
}

void skip_rest(const char *reason) {
    skip(reason, tap.expected_tests - tap.curr_test);
}

void diag (const char *msg) {
    TAP_PUTCHAR('#');
    TAP_PUTCHAR(' ');
    putstr(msg);
    TAP_PUTCHAR('\n');
}

int exit_status (void) {
    if (! tap.done_testing) {
        done_testing(tap.curr_test);
    }
#if defined(EXIT_SUCCESS) && defined(EXIT_FAILURE)
    return tap.is_passing ? EXIT_SUCCESS : EXIT_FAILURE;
#else
    return tap.is_passing ? 0 : -1;
#endif
}

#endif

#ifdef __cplusplus
}
#endif

#endif
