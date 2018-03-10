
# my *tiny* LuaJIT 2.1 fork

## Overview

I've worked in order to fill the compatibility gap between LuaJIT 2.1 and Lua 5.2/5.3/5.4.

Some of my Pull Requests were merged upstream by Mike Pall.

My unmerged work is available in the branch `tiny_fork` on <https://github.com/fperrad/LuaJIT>.
It contains:

  - [PR#269](https://github.com/LuaJIT/LuaJIT/pull/269) : backport string.\*pack\* from 5.3
  - [PR#327](https://github.com/LuaJIT/LuaJIT/pull/327) : backport lua\_rawlen, lua\_len & luaL\_len from 5.2
  - [PR#364](https://github.com/LuaJIT/LuaJIT/pull/364) : patterns better compatibility with 5.2 ('\0' in pattern)
  - optional `utf8` modules (5.3 & 5.4 variants) (rework of [PR#270](https://github.com/LuaJIT/LuaJIT/pull/270))
  - an optional `math` integer module (rework of [PR#313](https://github.com/LuaJIT/LuaJIT/pull/313))

## Pull Requests

- [PR#265](https://github.com/LuaJIT/LuaJIT/pull/265) : backport package.searchers from 5.2

    status : Merged in v2.1.0-beta3

- [PR#266](https://github.com/LuaJIT/LuaJIT/pull/266) : io:read compat with 5.3

    status : Merged in v2.1.0-beta3

- [PR#267](https://github.com/LuaJIT/LuaJIT/pull/267) : backport table.move from 5.3

    status : Merged in v2.1.0-beta3 (fully reworked by Mike Pall)

- [PR#268](https://github.com/LuaJIT/LuaJIT/pull/268) : backport coroutine.isyieldable from 5.3

    status : Merged in v2.1.0-beta3

- [PR#269](https://github.com/LuaJIT/LuaJIT/pull/269) : backport string.\*pack\* from 5.3

    status : Rejected / Included in my fork

- [PR#270](https://github.com/LuaJIT/LuaJIT/pull/270) : backport utf8 library from 5.3

    status : Rejected / Reworked in my fork as optional Lua modules (5.3 & 5.4 variants)

- [PR#273](https://github.com/LuaJIT/LuaJIT/pull/273) : refactor table.maxn with LJLIB_LUA

    status : Rejected

- [PR#302](https://github.com/LuaJIT/LuaJIT/pull/302) : remove compatibility with Lua 5.0

    status : Merged in v2.1.0-beta3

- [PR#307](https://github.com/LuaJIT/LuaJIT/pull/307) : remove old compatibility macros

    status : Merged in v2.1.0-beta3

- [PR#309](https://github.com/LuaJIT/LuaJIT/pull/309) : package/module better compatibility with Lua 5.2

    status : Merged in v2.1.0-beta3

- [PR#312](https://github.com/LuaJIT/LuaJIT/pull/312) : backport operators from 5.3

    status : Pending (huge change, break bytecode compatibility, waiting for v2.2)

- [PR#313](https://github.com/LuaJIT/LuaJIT/pull/313) : backport math.type and math.tointeger from 5.3

    status : Rejected (incompatible numeric model) / Reworked in my fork as an optional Lua module

- [PR#314](https://github.com/LuaJIT/LuaJIT/pull/314) : backport some C/API from 5.2/5.3

    status : Merged in v2.1.0-beta3

- [PR#315](https://github.com/LuaJIT/LuaJIT/pull/315) : extract lib\_coro from lib\_base

    status : Rejected (break binary compatibility)

- [PR#317](https://github.com/LuaJIT/LuaJIT/pull/317) : add 5.3 operators as syntax sugar

    status : Bugged / needs some rework

- [PR#327](https://github.com/LuaJIT/LuaJIT/pull/327) : backport lua\_rawlen, lua\_len & luaL\_len from 5.2

    status : Pending / Included in my fork

- [PR#364](https://github.com/LuaJIT/LuaJIT/pull/364) : patterns better compatibility with 5.2 ('\0' in pattern)

    status : Pending / Included in my fork

- [PR#404](https://github.com/LuaJIT/LuaJIT/pull/404) : add luaJIT_compat52 symbol

    status : Pending (no feedback)

- [PR#423](https://github.com/LuaJIT/LuaJIT/pull/423) : allow assert to take a table object as error

    status : Merged in v2.1.0-??? (fully reworked by Mike Pall)

- [PR#525](https://github.com/LuaJIT/LuaJIT/pull/525) : backport more C/API from 5.2/5.4

    status : Pending (no feedback)

- [PR#590](https://github.com/LuaJIT/LuaJIT/pull/590) : backport warning system from 5.4

    status : Rejected

- [PR#591](https://github.com/LuaJIT/LuaJIT/pull/591) : backport 31bits utf8 from lexico 5.4

    status : Rejected

## Build / Install Instructions

From source, like with LuaJIT, see <http://luajit.org/install.html>.

Note: [hererocks](https://github.com/luarocks/hererocks) could be used like this :

```
$ hererocks somewhere --luajit https://github.com/fperrad/LuaJIT.git@tiny_fork --compat 5.2
```

## Usage and Limitations

The optional `utf8` module must be explicitly loaded by:
```lua
utf8 = require'compat53.utf8'
```
or
```lua
utf8 = require'compat54.utf8'
```

After `require'compat53.math'`, the following functions and numbers are added in the `math` module:
- `math.maxinteger`
- `math.mininteger`
- `math.tointeger (n)`
- `math.type (n)`
- `math.ult (m, n)`

LuaJIT uses the same numeric type model as Lua 5.1 which is incompatible with Lua 5.3.
As a result, these functions work only in the range [-2^53, 2^53].
`math.maxinteger` and `math.mininteger` thus give the limits of this range.

## TODO

- Fix [PR#317](https://github.com/LuaJIT/LuaJIT/pull/317) : add 5.3 operators as syntax sugar
