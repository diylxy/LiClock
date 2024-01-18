#include <A_Config.h>

extern const char *err_invalid_param;

int peri_bmp_get(lua_State *L)
{
    float sea_press = weather.realtime.pressure;
    peripherals.load_append(PERIPHERALS_BMP280_BIT);
    if(lua_gettop(L) == 1)
    {
        if(lua_isnumber(L, 1))
        {
            sea_press = luaL_checknumber(L, 1);
        }
    }
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    if (peripherals.bmp.takeForcedMeasurement())
    {
        lua_pushnumber(L, peripherals.bmp.readPressure());
        lua_pushnumber(L, peripherals.bmp.readTemperature());
        lua_pushnumber(L, peripherals.bmp.readAltitude(sea_press / 100));
        xSemaphoreGive(peripherals.i2cMutex);
    }
    else
    {
        xSemaphoreGive(peripherals.i2cMutex);
        lua_pushstring(L, "测量失败");
        return 1;
    }
    return 3;
}

static const luaL_Reg _lualib[] = {
    {"get", peri_bmp_get},
    {NULL, NULL},
};

extern "C" int luaopen_peri_bmp(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
