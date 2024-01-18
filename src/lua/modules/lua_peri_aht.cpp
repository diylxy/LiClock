#include <A_Config.h>

extern const char *err_invalid_param;

int peri_aht_get(lua_State *L)
{
    sensors_event_t humidity, temp;
    peripherals.load_append(PERIPHERALS_AHT20_BIT);
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    peripherals.aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    xSemaphoreGive(peripherals.i2cMutex);
    lua_pushnumber(L, temp.temperature);
    lua_pushnumber(L, humidity.relative_humidity);
    return 2;
}

static const luaL_Reg _lualib[] = {
    {"get", peri_aht_get},
    {NULL, NULL},
};

extern "C" int luaopen_peri_aht(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
