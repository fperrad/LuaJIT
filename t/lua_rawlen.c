
#include "tap_lua.h"


static void test_boolean(const char *file, unsigned int line, lua_State *L, int n) {
    size_t len;

setup:
    lua_pushboolean(L, n);

exercise:
    len = lua_rawlen(L, -1);

verify:
    size_is(file, line, len, 0, "boolean");

teardown:
    lua_pop(L, 1);
}
#define TEST_BOOLEAN(n)         test_boolean(__FILE__, __LINE__, L, n)


static void test_function(const char *file, unsigned int line, lua_State *L, const char *s) {
    size_t len;

setup:
    lua_getglobal(L, s);
    assert(lua_isfunction(L, -1) == 1);

exercise:
    len = lua_objlen(L, -1);

verify:
    size_is(file, line, len, 0, s);

teardown:
    lua_pop(L, 1);
}
#define TEST_FUNCTION(s)        test_function(__FILE__, __LINE__, L, s)


static void test_lightuserdata(const char *file, unsigned int line, lua_State *L, void *p) {
    size_t len;

setup:
    lua_pushlightuserdata(L, p);

exercise:
    len = lua_objlen(L, -1);

verify:
    size_is(file, line, len, 0, "lightuserdata");

teardown:
    lua_pop(L, 1);
}
#define TEST_LIGHTUSERDATA(p)        test_lightuserdata(__FILE__, __LINE__, L, p)


static void test_number(const char *file, unsigned int line, lua_State *L, lua_Number n) {
    size_t len;

setup:
    lua_pushnumber(L, n);

exercise:
    len = lua_rawlen(L, -1);

verify:
    size_is(file, line, len, 0, "number");

teardown:
    lua_pop(L, 1);
}
#define TEST_NUMBER(n)          test_number(__FILE__, __LINE__, L, n)


static void test_string(const char *file, unsigned int line, lua_State *L, const char *s, size_t n) {
    size_t len;

setup:
    lua_pushlstring(L, s, n);

exercise:
    len = lua_rawlen(L, -1);

verify:
    size_is(file, line, len, n, s);

teardown:
    lua_pop(L, 1);
}
#define TEST_STRING(s, n)       test_string(__FILE__, __LINE__, L, s, n)


static void test_table(const char *file, unsigned int line, lua_State *L, size_t n) {
    size_t i, len;

setup:
    lua_createtable(L, n, 1);
    for (i = 1; i <= n; i++) {
        lua_pushnumber(L, 10.0 * n);
        lua_rawseti(L, -2, i);
    }
    lua_pushliteral(L, "foo");
    lua_pushliteral(L, "bar");
    lua_rawset(L, -3);

exercise:
    len = lua_rawlen(L, -1);

verify:
    size_is(file, line, len, n, "table");

teardown:
    lua_pop(L, 1);
}
#define TEST_TABLE(n)           test_table(__FILE__, __LINE__, L, n)


static void test_userdata(const char *file, unsigned int line, lua_State *L, size_t n) {
    size_t len;
    void *u;

setup:
    u = lua_newuserdata(L, n);
    assert(u != NULL);

exercise:
    len = lua_objlen(L, -1);

verify:
    size_is(file, line, len, n, "userdata");

teardown:
    lua_pop(L, 1);
}
#define TEST_USERDATA(n)        test_userdata(__FILE__, __LINE__, L, n)


int main(void) {
    TAP_LUA_INIT

    plan(11);

    TEST_BOOLEAN(1);
    TEST_FUNCTION("print");
    TEST_LIGHTUSERDATA(L);
    TEST_NUMBER(3.14);
    TEST_STRING("", 0);
    TEST_STRING("foo", 4);
    TEST_STRING("foo\0bar", 7);
    TEST_TABLE(0);
    TEST_TABLE(3);
    TEST_USERDATA(0);
    TEST_USERDATA(42);

    TAP_LUA_END
}

