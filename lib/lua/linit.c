/*
** $Id: linit.c $
** Initialization of libraries for lua.c and other clients
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

/*
** If you embed Lua in your program and need to open the standard
** libraries, call luaL_openlibs in your program. If you need a
** different set of libraries, copy this file to your project and edit
** it to suit your needs.
**
** You can also *preload* libraries, so that a later 'require' can
** open the library, which is already linked to the application.
** For that, do the following code:
**
**  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
**  lua_pushcfunction(L, luaopen_modname);
**  lua_setfield(L, -2, modname);
**  lua_pop(L, 1);  // remove PRELOAD table
*/

#include "lprefix.h"


#include <stddef.h>

#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"

int luaopen_appmanager(lua_State *L);
int luaopen_buzzer(lua_State *L);
int luaopen_display(lua_State *L);
int luaopen_gui(lua_State *L);
int luaopen_hal(lua_State *L);
int luaopen_peri_aht(lua_State *L);
int luaopen_peri_bmp(lua_State *L);
int luaopen_peri_sgp(lua_State *L);
int luaopen_weather(lua_State *L);
int luaopen_http(lua_State *L);
/*
** these libs are loaded by lua.c and are readily available to any Lua
** program
*/
static const luaL_Reg loadedlibs[] = {
  {LUA_GNAME, luaopen_base},
  {LUA_LOADLIBNAME, luaopen_package},
  {LUA_COLIBNAME, luaopen_coroutine},
  {LUA_TABLIBNAME, luaopen_table},
  {LUA_IOLIBNAME, luaopen_io},
  {LUA_OSLIBNAME, luaopen_os},
  {LUA_STRLIBNAME, luaopen_string},
  {LUA_MATHLIBNAME, luaopen_math},
  {LUA_UTF8LIBNAME, luaopen_utf8},
  {LUA_DBLIBNAME, luaopen_debug},
  {"appManager", luaopen_appmanager},
  {"buzzer", luaopen_buzzer},
  {"display", luaopen_display},
  {"gui", luaopen_gui},
  {"hal", luaopen_hal},
  {"aht", luaopen_peri_aht},
  {"bmp", luaopen_peri_bmp},
  {"sgp", luaopen_peri_sgp},
  {"weather", luaopen_weather},
  {"http", luaopen_http},
  {NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L) {
  const luaL_Reg *lib;
  /* "require" functions from 'loadedlibs' and set results to global table */
  for (lib = loadedlibs; lib->func; lib++) {
    luaL_requiref(L, lib->name, lib->func, 1);
    lua_pop(L, 1);  /* remove lib */
  }
}

