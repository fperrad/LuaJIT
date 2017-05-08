
#include "tap_lua.h"

//#define DUMP_STACK()    dump_stack(L)
#define DUMP_STACK()

static int wrap_len(lua_State *L) {
    assert(lua_gettop(L) == 1);
    lua_len(L, 1);
    return 1;
}


static int ret_len;
static int meta_len(lua_State *L) {
    diag("meta_len");
    lua_pushinteger(L, ret_len);
    return 1;
}


static void createmeta(lua_State*L, int n){
    ret_len = n;
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, meta_len);
    lua_setfield(L,-2,"__len");
    lua_setmetatable(L, -2);
}


static void test_boolean(const char *file, unsigned int line, lua_State *L, int n) {
    int res;

setup:
    lua_pushcfunction(L, wrap_len);
    lua_pushboolean(L, n);
    DUMP_STACK();

exercise:
    res = lua_pcall(L, 1, 1, 0);

verify:
    int_is(file, line, res, LUA_ERRRUN, "boolean");
    diag(lua_tostring(L, -1));

teardown:
    DUMP_STACK();
    lua_pop(L, 1);
}
#define TEST_BOOLEAN(n)         test_boolean(__FILE__, __LINE__, L, n)


static void test_function(const char *file, unsigned int line, lua_State *L, const char *s) {
    int res;

setup:
    lua_pushcfunction(L, wrap_len);
    lua_getglobal(L, s);
    assert(lua_isfunction(L, -1) == 1);
    DUMP_STACK();

exercise:
    res = lua_pcall(L, 1, 1, 0);

verify:
    int_is(file, line, res, LUA_ERRRUN, "function");
    diag(lua_tostring(L, -1));

teardown:
    DUMP_STACK();
    lua_pop(L, 1);
}
#define TEST_FUNCTION(s)        test_function(__FILE__, __LINE__, L, s)


static void test_lightuserdata(const char *file, unsigned int line, lua_State *L, void *p) {
    int res;

setup:
    lua_pushcfunction(L, wrap_len);
    lua_pushlightuserdata(L, p);
    DUMP_STACK();

exercise:
    res = lua_pcall(L, 1, 1, 0);

verify:
    int_is(file, line, res, LUA_ERRRUN, "lightuserdata");
    diag(lua_tostring(L, -1));

teardown:
    DUMP_STACK();
    lua_pop(L, 1);
}
#define TEST_LIGHTUSERDATA(p)        test_lightuserdata(__FILE__, __LINE__, L, p)


static void test_number(const char *file, unsigned int line, lua_State *L, lua_Number n) {
    int res;

setup:
    lua_pushcfunction(L, wrap_len);
    lua_pushnumber(L, n);
    DUMP_STACK();

exercise:
    res = lua_pcall(L, 1, 1, 0);

verify:
    int_is(file, line, res, LUA_ERRRUN, "number");
    diag(lua_tostring(L, -1));

teardown:
    DUMP_STACK();
    lua_pop(L, 1);
}
#define TEST_NUMBER(n)          test_number(__FILE__, __LINE__, L, n)


static void test_string(const char *file, unsigned int line, lua_State *L, const char *s, int n) {
    int len;

setup:
    lua_pushlstring(L, s, n);
    DUMP_STACK();

exercise:
    lua_len(L, -1); 

verify:
    assert(lua_isnumber(L, -1) == 1);
    len = lua_tointeger(L, -1);
    int_is(file, line, len, n, s);

teardown:
    DUMP_STACK();
    lua_pop(L, 2);
}
#define TEST_STRING(s, n)       test_string(__FILE__, __LINE__, L, s, n)


static void test_table(const char *file, unsigned int line, lua_State *L, int n) {
    int i, len;

setup:
    lua_createtable(L, n, 1);
    for (i = 1; i <= n; i++) {
        lua_pushnumber(L, 10.0 * n);
        lua_rawseti(L, -2, i);
    }
    lua_pushliteral(L, "foo");
    lua_pushliteral(L, "bar");
    lua_rawset(L, -3);
    DUMP_STACK();

exercise:
    lua_len(L, -1); 

verify:
    assert(lua_isnumber(L, -1) == 1);
    len = lua_tointeger(L, -1);
    int_is(file, line, len, n, "table");

teardown:
    DUMP_STACK();
    lua_pop(L, 2);
}
#define TEST_TABLE(n)           test_table(__FILE__, __LINE__, L, n)


static void test_table_meta_len(const char *file, unsigned int line, lua_State *L, int n) {
    int len;

setup:
    lua_createtable(L, n, 1);
    lua_pushliteral(L, "foo");
    lua_pushliteral(L, "bar");
    lua_rawset(L, -3);
    createmeta(L, n);
    DUMP_STACK();

exercise:
    lua_len(L, -1); 

verify:
    assert(lua_isnumber(L, -1) == 1);
    len = lua_tointeger(L, -1);
    int_is(file, line, len, n, "table with meta len");

teardown:
    DUMP_STACK();
    lua_pop(L, 2);
}
#define TEST_TABLE_META_LEN(n)          test_table_meta_len(__FILE__, __LINE__, L, n)


static void test_userdata(const char *file, unsigned int line, lua_State *L, size_t n) {
    void *u;
    int res;

setup:
    lua_pushcfunction(L, wrap_len);
    u = lua_newuserdata(L, n);
    assert(u != NULL);
    DUMP_STACK();

exercise:
    res = lua_pcall(L, 1, 1, 0);

verify:
    int_is(file, line, res, LUA_ERRRUN, "userdata without metamethod");
    diag(lua_tostring(L, -1));

teardown:
    DUMP_STACK();
    lua_pop(L, 1);
}
#define TEST_USERDATA(n)        test_userdata(__FILE__, __LINE__, L, n)


static void test_userdata_meta_len(const char *file, unsigned int line, lua_State *L, size_t n) {
    void *u;
    int len;

setup:
    u = lua_newuserdata(L, n);
    assert(u != NULL);
    createmeta(L, n);
    DUMP_STACK();

exercise:
    lua_len(L, -1); 

verify:
    assert(lua_isnumber(L, -1) == 1);
    len = lua_tointeger(L, -1);
    int_is(file, line, len, n, "userdata with meta len");

teardown:
    DUMP_STACK();
    lua_pop(L, 2);
}
#define TEST_USERDATA_META_LEN(n)       test_userdata_meta_len(__FILE__, __LINE__, L, n)


int main(void) {
    TAP_LUA_INIT

    plan(15);

    TEST_BOOLEAN(1);
    TEST_FUNCTION("print");
    TEST_LIGHTUSERDATA(L);
    TEST_NUMBER(3.14);
    TEST_STRING("", 0);
    TEST_STRING("foo", 3);
    TEST_STRING("foo\0bar", 7);
    TEST_TABLE(0);
    TEST_TABLE(3);
    TEST_TABLE_META_LEN(0);
    todo("work only with LUAJIT_ENABLE_LUA52COMPAT", 1);
    TEST_TABLE_META_LEN(42);
    TEST_USERDATA(0);
    TEST_USERDATA(42);
    TEST_USERDATA_META_LEN(14);
    TEST_USERDATA_META_LEN(0);

    TAP_LUA_END
}

