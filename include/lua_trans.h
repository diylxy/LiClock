#pragma once
#include "lua.hpp"

extern lua_State *L;

extern void openLua();
extern void lua_execute(const char *filename);
extern void closeLua();