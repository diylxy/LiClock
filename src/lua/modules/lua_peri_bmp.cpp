#include <A_Config.h>

extern const char *err_invalid_param;

int peri_bmp_get(lua_State *L)
{
    float sea_press = weather.realtime.pressure;
    if(lua_gettop(L) == 1)
    {
        if(lua_isnumber(L, 1))
        {
            sea_press = luaL_checknumber(L, 1);
        }
    }
    if (peripherals.bmp.takeForcedMeasurement())
    {
        lua_pushnumber(L, peripherals.bmp.readPressure());
        lua_pushnumber(L, peripherals.bmp.readTemperature());
        lua_pushnumber(L, peripherals.bmp.readAltitude(sea_press / 100));
    }
    else
    {
        lua_pushstring(L, "测量失败");
        return 1;
    }
    return 3;
}

static const luaL_Reg _lualib[] = {
    {"get", peri_bmp_get},
    {NULL, NULL},
};

extern "C" int luaopen_peri_bmp(void)
{
    luaL_newlib(L, _lualib);
    return 1;
}
