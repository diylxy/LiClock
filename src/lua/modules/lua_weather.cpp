#include <A_Config.h>

extern const char *err_invalid_param;

int weather_refresh(lua_State *L)
{
    lua_printf("[警告]受内存限制，在Web服务器运行时无法更新天气");
    weather.refresh();
    return 0;
}

int weather_hour24(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int index = luaL_checkinteger(L, 1);
    if (index < 0 || index > 20)
    {
        lua_pushstring(L, "参数错误");
        lua_error(L);
        return 0;
    }
    weather.getRealIndex(hal.timeinfo.tm_hour, &index);
    lua_newtable(L);
    lua_pushstring(L, weather.hour24[index].date);
    lua_setfield(L, -2, "date");
    lua_pushinteger(L, weather.hour24[index].weathernum);
    lua_setfield(L, -2, "weathernum");
    lua_pushinteger(L, weather.hour24[index].temperature);
    lua_setfield(L, -2, "temperature");
    lua_pushinteger(L, weather.hour24[index].windspeed);
    lua_setfield(L, -2, "windspeed");
    lua_pushinteger(L, weather.hour24[index].winddirection);
    lua_setfield(L, -2, "winddirection");
    lua_pushinteger(L, weather.hour24[index].rain);
    lua_setfield(L, -2, "rain");
    lua_pushinteger(L, weather.hour24[index].humidity);
    lua_setfield(L, -2, "humidity");
    return 1;
}

int weather_rain(lua_State *L)
{
    lua_settop(L, 0);
    lua_newtable(L);
    for (int i = 0; i < 120; i++)
    {
        lua_pushinteger(L, weather.rain[i]);
        lua_rawseti(L, -2, i);
    }
    return 1;
}

int weather_five_days(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int i = luaL_checkinteger(L, 1);
    lua_newtable(L);
    lua_pushinteger(L, weather.five_days[i].weathernum);
    lua_setfield(L, -2, "weathernum");
    lua_pushinteger(L, weather.five_days[i].max);
    lua_setfield(L, -2, "max");
    lua_pushinteger(L, weather.five_days[i].min);
    lua_setfield(L, -2, "min");
    return 1;
}

int weather_realtime(lua_State *L)
{
    lua_settop(L, 0);
    lua_newtable(L);
    lua_pushinteger(L, weather.realtime.weathernum);
    lua_setfield(L, -2, "weathernum");
    lua_pushinteger(L, weather.realtime.temperature);
    lua_setfield(L, -2, "temperature");
    lua_pushinteger(L, weather.realtime.humidity);
    lua_setfield(L, -2, "humidity");
    lua_pushnumber(L, weather.realtime.pressure);
    lua_setfield(L, -2, "pressure");
    return 1;
}

int weather_desc1(lua_State *L)
{
    lua_pushstring(L, weather.desc1);
    return 1;
}

int weather_desc2(lua_State *L)
{
    lua_pushstring(L, weather.desc2);
    return 1;
}

int weather_hasAlert(lua_State *L)
{
    lua_pushboolean(L, weather.hasAlert);
    return 1;
}

int weather_alert(lua_State *L)
{
    lua_pushstring(L, weather.alert);
    return 1;
}

int weather_alertTitle(lua_State *L)
{
    lua_pushstring(L, weather.alertTitle);
    return 1;
}

int weather_alertPubTime(lua_State *L)
{
    lua_pushinteger(L, weather.alertPubTime);
    return 1;
}

int weather_lastupdate(lua_State *L)
{
    lua_pushinteger(L, weather.lastupdate);
    return 1;
}

static const luaL_Reg _lualib[] = {
    {"refresh", weather_refresh},
    {"hour24", weather_hour24},
    {"rain", weather_rain},
    {"five_days", weather_five_days},
    {"realtime", weather_realtime},
    {"desc1", weather_desc1},
    {"desc2", weather_desc2},
    {"hasAlert", weather_hasAlert},
    {"alert", weather_alert},
    {"alertTitle", weather_alertTitle},
    {"alertPubTime", weather_alertPubTime},
    {"lastupdate", weather_lastupdate},
    {NULL, NULL},
};

extern "C" int luaopen_weather(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
