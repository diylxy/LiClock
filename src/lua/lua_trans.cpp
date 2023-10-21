#include "A_Config.h"

const char *err_invalid_param = "参数个数不符";
lua_State *L = NULL; // 公用Lua虚拟机状态
extern char __mypath[101];

static int common_delay(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int ms = luaL_checkinteger(L, 1);
    delay(ms);
    return 0;
}

static int common_NextWakeup(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int nwu = luaL_checkinteger(L, 1);
    appManager.nextWakeup = nwu;
    return 0;
}

static int common_digitalRead(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int pin = luaL_checkinteger(L, 1);
    int ret = digitalRead(pin);
    lua_pushinteger(L, ret);
    return 1;
}

static int common_digitalWrite(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int pin = luaL_checkinteger(L, 1);
    int val = luaL_checkinteger(L, 2);
    digitalWrite(pin, val);
    return 0;
}

static int common_analogRead(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int pin = luaL_checkinteger(L, 1);
    int ret = analogRead(pin);
    lua_pushinteger(L, ret);
    return 1;
}

static int common_pinMode(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int pin = luaL_checkinteger(L, 1);
    int mode = luaL_checkinteger(L, 2);
    pinMode(pin, mode);
    return 0;
}

void openLua_simple()
{
    if (L)
        return;
    Serial.println("Lua 部分初始化");
    L = luaL_newstate();
}

void openLua()
{
    if (L)
        return;
    Serial.println("Lua 初始化");
    L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, common_delay);
    lua_setglobal(L, "delay");
    lua_pushcfunction(L, common_NextWakeup);
    lua_setglobal(L, "nextWakeup");
    lua_pushcfunction(L, common_digitalRead);
    lua_setglobal(L, "digitalRead");
    lua_pushcfunction(L, common_digitalWrite);
    lua_setglobal(L, "digitalWrite");
    lua_pushcfunction(L, common_analogRead);
    lua_setglobal(L, "analogRead");
    lua_pushcfunction(L, common_pinMode);
    lua_setglobal(L, "pinMode");
}

void lua_execute(const char *filename)
{
    if (luaL_dofile(L, filename))
    {
        lua_printf("运行错误: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    // luaL_dostring(L, "display.clearScreen()\ndisplay.display()\nprint(\"lua_execute\")\nbuzzer.append(1000, 100)\n");
}
void closeLua()
{
    if (L)
        lua_close(L);
    L = NULL;
}
