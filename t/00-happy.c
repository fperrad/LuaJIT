
#include "tap_lua.h"

int main(void) {
    TAP_LUA_INIT

    plan(4);

    PTR_NOT_NULL( L, "lua_State opened" );
    lua_getglobal(L, "print");
    INT_IS( lua_isfunction(L, -1), 1, "function on top" );
    lua_pushliteral(L, "# Happy testing in Lua.");
    INT_IS( lua_isstring(L, -1), 1, "string on top" );
    lua_call(L, 1, 0);
    PASS( "Happy !" );

    TAP_LUA_END
}

