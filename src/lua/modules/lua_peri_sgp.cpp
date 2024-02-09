#include <A_Config.h>

extern const char *err_invalid_param;

int sgp_get(lua_State *L)
{
    peripherals.load_append(PERIPHERALS_SGP30_BIT);
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    if (peripherals.sgp.IAQmeasure())
    {
        lua_pushinteger(L, peripherals.sgp.TVOC);
        lua_pushinteger(L, peripherals.sgp.eCO2);
        xSemaphoreGive(peripherals.i2cMutex);
    }
    else
    {
        lua_pushstring(L, "测量失败");
        return 1;
    }
    return 2;
}

int sgp_getRaw(lua_State *L)
{
    peripherals.load_append(PERIPHERALS_SGP30_BIT);
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    if (peripherals.sgp.IAQmeasureRaw())
    {
        lua_pushinteger(L, peripherals.sgp.rawH2);
        lua_pushinteger(L, peripherals.sgp.rawEthanol);
        xSemaphoreGive(peripherals.i2cMutex);
    }
    else
    {
        xSemaphoreGive(peripherals.i2cMutex);
        lua_pushstring(L, "测量失败");
        return 1;
    }
    return 2;
}

int sgp_getIAQBaseline(lua_State *L)
{
    uint16_t TVOC_base, eCO2_base;
    peripherals.load_append(PERIPHERALS_SGP30_BIT);
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    if (peripherals.sgp.getIAQBaseline(&eCO2_base, &TVOC_base))
    {
        lua_pushinteger(L, eCO2_base);
        lua_pushinteger(L, TVOC_base);
        xSemaphoreGive(peripherals.i2cMutex);
    }
    else
    {
        xSemaphoreGive(peripherals.i2cMutex);
        lua_pushstring(L, "获取失败");
        return 1;
    }
    return 2;
}

int sgp_setIAQBaseline(lua_State *L)
{
    peripherals.load_append(PERIPHERALS_SGP30_BIT);
    if (lua_gettop(L) == 2)
    {
        if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
        {
            uint16_t eCO2_base = luaL_checkinteger(L, 1);
            uint16_t TVOC_base = luaL_checkinteger(L, 2);
            xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
            if (peripherals.sgp.setIAQBaseline(eCO2_base, TVOC_base))
            {
                xSemaphoreGive(peripherals.i2cMutex);
                return 0;
            }
            else
            {
                xSemaphoreGive(peripherals.i2cMutex);
                lua_pushstring(L, "设置失败");
                lua_error(L);
                return 0;
            }
        }
    }
    return 0;
}

static uint32_t getAbsoluteHumidity(float temperature, float humidity)
{
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity);                                                                // [mg/m^3]
    return absoluteHumidityScaled;
}

int sgp_setHumidity(lua_State *L)
{
    peripherals.load_append(PERIPHERALS_SGP30_BIT);
    if (lua_gettop(L) == 2)
    {
        if (lua_isnumber(L, 1) && lua_isnumber(L, 2))
        {
            float temperature = luaL_checknumber(L, 1);
            float humidity = luaL_checknumber(L, 2);
            peripherals.sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
            lua_pushboolean(L, true);
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

static const luaL_Reg _lualib[] = {
    {"get", sgp_get},
    {"getRaw", sgp_getRaw},
    {"getIAQBaseline", sgp_getIAQBaseline},
    {"setIAQBaseline", sgp_setIAQBaseline},
    {"setHumidity", sgp_setHumidity},
    {NULL, NULL},
};

extern "C" int luaopen_peri_sgp(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
