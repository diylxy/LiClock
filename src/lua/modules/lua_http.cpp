#include <A_Config.h>

extern const char *err_invalid_param;
HTTPClient lua_http;
HTTPClient lua_https;
bool last_is_https = false;
int last_code = 0;

int lua_http_begin(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    const char *url = luaL_checkstring(L, 1);
    if(strlen(url) <= 7)
    {
        lua_pushstring(L, "地址需要以http或https开头");
        lua_error(L);
    }
    if(url[4] == 's')
    {
        Serial.printf("使用https\n");
        last_is_https = true;
        lua_https.begin(url);
    }
    else
    {
        last_is_https = false;
        lua_http.begin(url);
    }
    return 0;
}
int lua_http_addHeader(lua_State *L)
{
    const char *name = luaL_checkstring(L, 1);
    const char *value = luaL_checkstring(L, 2);
    bool first = false;
    bool replace = true;
    if(lua_isboolean(L, 3))
        first = lua_toboolean(L, 3);
    if(lua_isboolean(L, 4))
        replace = lua_toboolean(L, 4);
    if(last_is_https)
    {
        lua_https.addHeader(name, value, first, replace);
    }
    else
    {
        lua_http.addHeader(name, value, first, replace);
    }
    return 0;
}

int lua_http_request(lua_State *L)
{
    if (lua_gettop(L) != 1 && lua_gettop(L) != 2)
    {
        lua_pushstring(L, "参数：方法,payload");
        lua_error(L);
        return 0;
    }
    const char *method = luaL_checkstring(L, 1);
    const char *payload = luaL_checkstring(L, 2);

    if(last_is_https)
    {
        last_code = lua_https.sendRequest(method, payload);
    }
    else
    {
        last_code = lua_http.sendRequest(method, payload);
    }
    return 0;
}

int lua_http_text(lua_State *L)
{
    if(last_is_https)
    {
        lua_pushstring(L, lua_https.getString().c_str());
    }
    else
    {
        lua_pushstring(L, lua_http.getString().c_str());
    }
    return 1;
}
int lua_http_code(lua_State *L)
{
    lua_pushinteger(L, last_code);
    return 1;
}
int lua_http_end(lua_State *L)
{
    lua_http.end();
    lua_https.end();
    return 0;
}
static const luaL_Reg _lualib[] = {
    {"begin", lua_http_begin},
    {"addHeader", lua_http_addHeader},
    {"request", lua_http_request},
    {"text", lua_http_text},
    {"code", lua_http_code},
    {"stop", lua_http_end},             //不能用end
    {NULL, NULL},
};

extern "C" int luaopen_http(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}