#include <A_Config.h>

extern const char *err_invalid_param;

int buzzer_append(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int freq = luaL_checkinteger(L, 1);
    int duration = luaL_checkinteger(L, 2);
    buzzer.append(freq, duration);
    return 0;
}

int buzzer_playFile(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    const char *path = luaL_checkstring(L, 1);
    buzzer.playFile(path);
    return 0;
}

int buzzer_forceStop(lua_State *L)
{
    buzzer.forceStop();
    return 0;
}

static const luaL_Reg _lualib[] = {
    {"append", buzzer_append},
    {"playFile", buzzer_playFile},
    {"forceStop", buzzer_forceStop},
    {NULL, NULL},
};

extern "C" int luaopen_buzzer(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
