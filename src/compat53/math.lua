----------------------------------------------------------------------------
-- math integer library.
--
-- Copyright (C) 2019 Francois Perrad
--
-- This library is licensed under the terms of the MIT/X11 license,
-- like LuaJIT itself.
----------------------------------------------------------------------------

local error, type = error, type
local math = math

math.maxinteger = 9007199254740992      -- 2^53
math.mininteger = -9007199254740992

function math.tointeger (n)
  if type(n) == 'number' and n <= 9007199254740992 and n >= -9007199254740992 and n % 1 == 0 then
    return n
  end
  return nil
end

function math.type (n)
  if type(n) == 'number' then
    if n <= 9007199254740992 and n >= -9007199254740992 and n % 1 == 0 then
      return 'integer'
    else
      return 'float'
    end
  else
    return nil
  end
end

function math.ult (m, n)
  local t = type(m)
  if t ~= 'number' then
    error("bad argument #1 to 'ult' (number expected, got " .. t .. ")", 2)
  end
  if m > 9007199254740992 or m < -9007199254740992 or m % 1 ~= 0 then
    error("bad argument #1 to 'ult' (number has no integer representation)", 2)
  end
  t = type(n)
  if t ~= 'number' then
    error("bad argument #2 to 'ult' (number expected, got " .. t .. ")", 2)
  end
  if n > 9007199254740992 or n < -9007199254740992 or n % 1 ~= 0 then
    error("bad argument #2 to 'ult' (number has no integer representation)", 2)
  end
  if m >= 0 and n < 0 then
    return true
  elseif m < 0 and n >= 0 then
    return false
  else
    return m < n
  end
end
