
#include <assert.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "tap_helpers.h"


#define TAP_LUA_INIT \
    lua_State *L = luaL_newstate(); \
    assert(L != NULL); \
    luaL_openlibs(L);

#define TAP_LUA_END \
    lua_close(L); \
    return exit_status();

extern void dump(lua_State *L, int i);
extern void dump_stack(lua_State *L);

#ifndef TAP_LUA_NO_IMPL

void dump(lua_State *L, int i) {
    char buf[32];
    snprintf(buf, sizeof buf, "# % 3d: ", i);
    putstr(buf);
    switch (lua_type(L, i)) {
        case LUA_TNIL:
            putstr("nil");
            break;
        case LUA_TBOOLEAN:
            putstr("boolean\t");
            putstr(lua_toboolean(L, i) ? "true" : "false");
            break;
        case LUA_TLIGHTUSERDATA:
            snprintf(buf, sizeof buf, "lightuserdata\t%p", lua_touserdata(L, i));
            putstr(buf);
            break;
        case LUA_TNUMBER:
            snprintf(buf, sizeof buf, "number\t%g", lua_tonumber(L, i));
            putstr(buf);
            break;
        case LUA_TSTRING:
            putstr("string\t");
            putstr(lua_tostring(L, i));
            break;
        case LUA_TTABLE:
            snprintf(buf, sizeof buf, "table\t%p", lua_topointer(L, i));
            putstr(buf);
            break;
        case LUA_TFUNCTION:
            snprintf(buf, sizeof buf, "function\t%p", lua_topointer(L, i));
            putstr(buf);
            break;
        case LUA_TUSERDATA:
            snprintf(buf, sizeof buf, "userdata\t%p", lua_touserdata(L, i));
            putstr(buf);
            break;
        case LUA_TTHREAD:
            snprintf(buf, sizeof buf, "thread\t%p", lua_tothread(L, i));
            putstr(buf);
            break;
        default:
            assert(0);
    }
    TAP_PUTCHAR('\n');
}

void dump_stack(lua_State *L) {
    int i;
    int n = lua_gettop(L);
    if (n == 0) {
        putstr("# empty stack\n");
    }
    for (i = 1; i <= n; i++) {
        dump(L, i);
    }
    putstr("# ======= end =======\n");
}

#endif

