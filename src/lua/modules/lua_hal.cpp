#include <A_Config.h>

extern const char *err_invalid_param;

int hal_VCC(lua_State *L)
{
    int ret = hal.VCC;
    lua_pushinteger(L, ret);
    return 1;
}

int hal_USBPluggedIn(lua_State *L)
{
    bool ret = hal.USBPluggedIn;
    lua_pushboolean(L, ret);
    return 1;
}

int hal_isCharging(lua_State *L)
{
    bool ret = hal.isCharging;
    lua_pushboolean(L, ret);
    return 1;
}

int hal_now(lua_State *L)
{
    int ret = hal.now;
    lua_pushinteger(L, ret);
    return 1;
}

int hal_timeinfo(lua_State *L)
{
    lua_pushinteger(L, hal.timeinfo.tm_year + 1900);
    lua_pushinteger(L, hal.timeinfo.tm_mon);
    lua_pushinteger(L, hal.timeinfo.tm_mday);
    lua_pushinteger(L, hal.timeinfo.tm_wday);
    lua_pushinteger(L, hal.timeinfo.tm_hour);
    lua_pushinteger(L, hal.timeinfo.tm_min);
    lua_pushinteger(L, hal.timeinfo.tm_sec);
    return 7;
}

int hal_getTime(lua_State *L)
{
    hal.getTime();
    return 0;
}

int hal_autoConnectWiFi(lua_State *L)
{
    hal.autoConnectWiFi();
    return 0;
}

int hal_powerOff(lua_State *L)
{
    bool displayMessage = true;
    if (lua_gettop(L) == 1)
    {
        displayMessage = lua_toboolean(L, 1);
    }
    hal.powerOff(displayMessage);
    return 0;
}

int hal_reboot(lua_State *L)
{
    ESP.restart();
    return 0;
}

int hal_detachAllButtonEvents(lua_State *L)
{
    hal.detachAllButtonEvents();
    return 0;
}

static const luaL_Reg _lualib[] =
    {
        {"VCC", hal_VCC},
        {"USBPluggedIn", hal_USBPluggedIn},
        {"isCharging", hal_isCharging},
        {"now", hal_now},
        {"timeinfo", hal_timeinfo},
        {"getTime", hal_getTime},
        {"autoConnectWiFi", hal_autoConnectWiFi},
        {"powerOff", hal_powerOff},
        {"reboot", hal_reboot},
        {"detachAllButtonEvents", hal_detachAllButtonEvents},
        {NULL, NULL},
};

extern "C" int luaopen_hal(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
