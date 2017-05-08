/*
**  tap4embedded : http://github.com/fperrad/tap4embedded
**
**  Copyright (C) 2016 Francois Perrad.
**
**  tap4embedded is free software; you can redistribute it and/or modify it
**  under the terms of the Artistic License 2.0
*/

#ifndef TAP_HELPERS_H
#define TAP_HELPERS_H

#ifdef TAP_C99
#include <stdint.h>
#include <inttypes.h>
#endif
#include <string.h>
#include "tap.h"

#if defined(__unix__) || defined(_WIN32) || defined(_WIN64)
#define TAP_EXPLAIN 1
#else
#define TAP_EXPLAIN 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern void int_is(const char *file, unsigned int line, int got, int expected, const char *name);
#define INT_IS(got, expected, name)     int_is(__FILE__, __LINE__, (got), (expected), (name))
extern void long_is(const char *file, unsigned int line, long got, long expected, const char *name);
#define LONG_IS(got, expected, name)    long_is(__FILE__, __LINE__, (got), (expected), (name))
extern void ptr_is(const char *file, unsigned int line, void* got, void* expected, const char *name);
#define PTR_IS(got, expected, name)     ptr_is(__FILE__, __LINE__, (got), (expected), (name))
extern void size_is(const char *file, unsigned int line, size_t got, size_t expected, const char *name);
#define SIZE_IS(got, expected, name)    size_is(__FILE__, __LINE__, (got), (expected), (name))
extern void string_is(const char *file, unsigned int line, const char* got, const char* expected, const char *name);
#define STRING_IS(got, expected, name)  string_is(__FILE__, __LINE__, (got), (expected), (name))
extern void uint_is(const char *file, unsigned int line, unsigned int got, unsigned int expected, const char *name);
#define UINT_IS(got, expected, name)    uint_is(__FILE__, __LINE__, (got), (expected), (name))
extern void ulong_is(const char *file, unsigned int line, unsigned long got, unsigned long expected, const char *name);
#define ULONG_IS(got, expected, name)   ulong_is(__FILE__, __LINE__, (got), (expected), (name))
extern void ptr_not_null(const char *file, unsigned int line, void* got, const char *name);
#define PTR_NOT_NULL(got, name)         ptr_not_null(__FILE__, __LINE__, (got), (name))

#ifdef TAP_C99
extern void int8_is(const char *file, unsigned int line, int8_t got, int8_t expected, const char *name);
#define INT8_IS(got, expected, name)    int8_is(__FILE__, __LINE__, (got), (expected), (name))
extern void int16_is(const char *file, unsigned int line, int16_t got, int16_t expected, const char *name);
#define INT16_IS(got, expected, name)   int16_is(__FILE__, __LINE__, (got), (expected), (name))
extern void int32_is(const char *file, unsigned int line, int32_t got, int32_t expected, const char *name);
#define INT32_IS(got, expected, name)   int32_is(__FILE__, __LINE__, (got), (expected), (name))
extern void int64_is(const char *file, unsigned int line, int64_t got, int64_t expected, const char *name);
#define INT64_IS(got, expected, name)   int64_is(__FILE__, __LINE__, (got), (expected), (name))
extern void uint8_is(const char *file, unsigned int line, uint8_t got, uint8_t expected, const char *name);
#define UINT8_IS(got, expected, name)   uint8_is(__FILE__, __LINE__, (got), (expected), (name))
extern void uint16_is(const char *file, unsigned int line, uint16_t got, uint16_t expected, const char *name);
#define UINT16_IS(got, expected, name)  uint16_is(__FILE__, __LINE__, (got), (expected), (name))
extern void uint32_is(const char *file, unsigned int line, uint32_t got, uint32_t expected, const char *name);
#define UINT32_IS(got, expected, name)  uint32_is(__FILE__, __LINE__, (got), (expected), (name))
extern void uint64_is(const char *file, unsigned int line, uint64_t got, uint64_t expected, const char *name);
#define UINT64_IS(got, expected, name)  uint64_is(__FILE__, __LINE__, (got), (expected), (name))
#endif

#ifndef TAP_HELPERS_NO_IMPL

#define TAP_GOT         "#         got: "
#define TAP_EXPECTED    "#    expected: "

void string_is(const char *file, unsigned int line,
               const char* got, const char* expected, const char *name) {
    int ret = strcmp(got, expected);
    ok(file, line, ret == 0, name);
    if (ret != 0) {
        putstr(TAP_GOT);
        putstr(got);
        TAP_PUTCHAR('\n');
        putstr(TAP_EXPECTED);
        putstr(expected);
        TAP_PUTCHAR('\n');
    }
}

void ptr_not_null(const char *file, unsigned int line,
                  void* got, const char *name) {
    ok(file, line, got != NULL, name);
    if (got == NULL) {
        putstr(TAP_GOT "NULL\n");
        putstr(TAP_EXPECTED "anything else\n");
    }
}

#define NUM_IS1(func, type, fmt1) \
void func(const char *file, unsigned int line,                          \
          type got, type expected, const char *name) {                  \
    ok(file, line, got == expected, name);                              \
    if (got != expected) {                                              \
        char buf[16];                                                   \
        putstr(TAP_GOT);                                                \
        sprintf(buf, fmt1 "\n", got);                                   \
        putstr(buf);                                                    \
        putstr(TAP_EXPECTED);                                           \
        sprintf(buf, fmt1 "\n", expected);                              \
        putstr(buf);                                                    \
    }                                                                   \
}

#define NUM_IS2(func, type, fmt1, fmt2) \
void func(const char *file, unsigned int line,                          \
          type got, type expected, const char *name) {                  \
    ok(file, line, got == expected, name);                              \
    if (got != expected) {                                              \
        char buf[24];                                                   \
        putstr(TAP_GOT);                                                \
        sprintf(buf, fmt1 " (" fmt2 ")\n", got, got);                   \
        putstr(buf);                                                    \
        putstr(TAP_EXPECTED);                                           \
        sprintf(buf, fmt1 " (" fmt2 ")\n", expected, expected);         \
        putstr(buf);                                                    \
    }                                                                   \
}

#ifndef PRIuMAX
#if __WORDSIZE == 64
#define PRIuMAX "l"
#else
#define PRIuMAX
#endif
#endif

NUM_IS1(int_is, int, "%d")
NUM_IS1(long_is, long, "%ld")
NUM_IS1(ptr_is, void*, "%p")
NUM_IS1(size_is, size_t, "%" PRIuMAX "u")
NUM_IS2(uint_is, unsigned int, "0x%X", "%u")
NUM_IS2(ulong_is, unsigned long, "0x%lX", "%lu")

#ifdef TAP_C99
#if __WORDSIZE == 64
#define TAP_PRI64_PREFIX "l"
#else
#define TAP_PRI64_PREFIX "ll"
#endif
#ifndef PRId64
#define PRId64 TAP_PRI64_PREFIX "d"
#endif
#ifndef PRIu64
#define PRIu64 TAP_PRI64_PREFIX "u"
#endif
#ifndef PRIx64
#define PRIx64 TAP_PRI64_PREFIX "x"
#endif

NUM_IS1(int8_is, int8_t, "%" PRId8)
NUM_IS1(int16_is, int16_t, "%" PRId16)
NUM_IS1(int32_is, int32_t, "%" PRId32)
NUM_IS1(int64_is, int64_t, "%" PRId64)
NUM_IS2(uint8_is, uint8_t, "0x%02" PRIx8 , "%" PRIu8)
NUM_IS2(uint16_is, uint16_t, "0x%04" PRIx16, "%" PRIu16)
NUM_IS2(uint32_is, uint32_t, "0x%08" PRIx32, "%" PRIu32)
NUM_IS2(uint64_is, uint64_t, "0x%016" PRIx64, "%" PRIu64)
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
