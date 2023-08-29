#include "A_Config.h"
extern const char *err_invalid_param;
static int display_clearscreen(lua_State *L)
{
    if (lua_gettop(L) == 1)
    {
        int color = lua_tointeger(L, 1);
        display.clearScreen(color);
        return 0;
    }
    display.clearScreen();
    return 0;
}

static int display_display(lua_State *L)
{
    bool part = lua_toboolean(L, 1);
    display.display(part);
    return 0;
}

static int display_displayWindow(lua_State *L)
{
    if (lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    display.displayWindow(x, y, w, h);
    return 0;
}
static int display_setFullWindow(lua_State *L)
{
    display.setFullWindow();
    return 0;
}

static int display_setPartialWindow(lua_State *L)
{
    if (lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    display.setPartialWindow(x, y, w, h);
    return 0;
}

static int display_drawPixel(lua_State *L)
{
    if (lua_gettop(L) != 3)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int color = luaL_checkinteger(L, 3);
    display.drawPixel(x, y, color);
    return 0;
}

static int display_drawLine(lua_State *L)
{
    if (lua_gettop(L) != 5)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    int color = luaL_checkinteger(L, 5);
    display.drawLine(x0, y0, x1, y1, color);
    return 0;
}

static int display_drawRect(lua_State *L)
{
    if (lua_gettop(L) != 5)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int color = luaL_checkinteger(L, 5);
    display.drawRect(x, y, w, h, color);
    return 0;
}

static int display_fillRect(lua_State *L)
{
    if (lua_gettop(L) != 5)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int color = luaL_checkinteger(L, 5);
    display.fillRect(x, y, w, h, color);
    return 0;
}

static int display_drawCircle(lua_State *L)
{
    if (lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int r = luaL_checkinteger(L, 3);
    int color = luaL_checkinteger(L, 4);
    display.drawCircle(x, y, r, color);
    return 0;
}

static int display_fillCircle(lua_State *L)
{
    if (lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int r = luaL_checkinteger(L, 3);
    int color = luaL_checkinteger(L, 4);
    display.fillCircle(x, y, r, color);
    return 0;
}

static int display_drawTriangle(lua_State *L)
{
    if (lua_gettop(L) != 7)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    int x2 = luaL_checkinteger(L, 5);
    int y2 = luaL_checkinteger(L, 6);
    int color = luaL_checkinteger(L, 7);
    display.drawTriangle(x0, y0, x1, y1, x2, y2, color);
    return 0;
}

static int display_fillTriangle(lua_State *L)
{
    if (lua_gettop(L) != 7)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x0 = luaL_checkinteger(L, 1);
    int y0 = luaL_checkinteger(L, 2);
    int x1 = luaL_checkinteger(L, 3);
    int y1 = luaL_checkinteger(L, 4);
    int x2 = luaL_checkinteger(L, 5);
    int y2 = luaL_checkinteger(L, 6);
    int color = luaL_checkinteger(L, 7);
    display.fillTriangle(x0, y0, x1, y1, x2, y2, color);
    return 0;
}

static int display_drawRoundRect(lua_State *L)
{
    if (lua_gettop(L) != 6)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int r = luaL_checkinteger(L, 5);
    int color = luaL_checkinteger(L, 6);
    display.drawRoundRect(x, y, w, h, r, color);
    return 0;
}

static int display_fillRoundRect(lua_State *L)
{
    if (lua_gettop(L) != 6)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int r = luaL_checkinteger(L, 5);
    int color = luaL_checkinteger(L, 6);
    display.fillRoundRect(x, y, w, h, r, color);
    return 0;
}

static int display_setCursor(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    display.setCursor(x, y);
    u8g2Fonts.setCursor(x, y);
    return 0;
}

static int display_setFont(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int fontid = luaL_checkinteger(L, 1);
    if ((uint32_t)fontid >= 3)
        fontid = 0;
    if (fontid == 0)
    {
        display.setFont(&FreeSans9pt7b);
    }
    else if (fontid == 1)
    {
        display.setFont(&FreeSans18pt7b);
    }
    else if (fontid == 2)
    {
        display.setFont(&Picopixel);
    }
    return 0;
}

static int display_setTextColor(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int color = luaL_checkinteger(L, 1);
    display.setTextColor(color);
    u8g2Fonts.setForegroundColor(color);
    return 0;
}

static int display_setBackgroundColor(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int color = luaL_checkinteger(L, 1);
    u8g2Fonts.setBackgroundColor(color);
    return 0;
}

static int display_print(lua_State *L)
{
    const char *str = luaL_checkstring(L, 1);
    display.print(str);
    return 0;
}

static int display_getCursorX(lua_State *L)
{
    int x;
    x = display.getCursorX();
    lua_pushinteger(L, x);
    return 1;
}

static int display_getCursorY(lua_State *L)
{
    int y;
    y = display.getCursorY();
    lua_pushinteger(L, y);
    return 1;
}

static int display_u8g2GetCursorX(lua_State *L)
{
    int x;
    x = u8g2Fonts.getCursorX();
    lua_pushinteger(L, x);
    return 1;
}

static int display_u8g2GetCursorY(lua_State *L)
{
    int y;
    y = u8g2Fonts.getCursorY();
    lua_pushinteger(L, y);
    return 1;
}

static int display_u8g2Print(lua_State *L)
{
    const char *str = luaL_checkstring(L, 1);
    u8g2Fonts.print(str);
    return 0;
}

static int display_drawLBM(lua_State *L)
{
    if (lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    const char *filename = luaL_checkstring(L, 3);
    int color = luaL_checkinteger(L, 4);
    GUI::drawLBM(x, y, filename, color);
    return 0;
}

static const luaL_Reg _lualib[] = {
    {"clearScreen", display_clearscreen},
    {"display", display_display},
    {"displayWindow", display_displayWindow},
    {"setFullWindow", display_setFullWindow},
    {"setPartialWindow", display_setPartialWindow},
    {"drawPixel", display_drawPixel},
    {"drawLine", display_drawLine},
    {"drawRect", display_drawRect},
    {"fillRect", display_fillRect},
    {"drawCircle", display_drawCircle},
    {"fillCircle", display_fillCircle},
    {"drawTriangle", display_drawTriangle},
    {"fillTriangle", display_fillTriangle},
    {"drawRoundRect", display_drawRoundRect},
    {"fillRoundRect", display_fillRoundRect},
    {"setCursor", display_setCursor},
    {"setFont", display_setFont},
    {"setTextColor", display_setTextColor},
    {"setBackgroundColor", display_setBackgroundColor},
    {"print", display_print},
    {"getCursorX", display_getCursorX},
    {"getCursorY", display_getCursorY},
    {"u8g2GetCursorX", display_u8g2GetCursorX},
    {"u8g2GetCursorY", display_u8g2GetCursorY},
    {"u8g2Print", display_u8g2Print},
    {"drawLBM", display_drawLBM},
    {NULL, NULL},
};

extern "C" extern "C" int luaopen_display(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}
/**
 *
 *
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, (lua_Number)HUGE_VAL);
  lua_setfield(L, -2, "huge");
  lua_pushinteger(L, LUA_MAXINTEGER);
  lua_setfield(L, -2, "maxinteger");
  lua_pushinteger(L, LUA_MININTEGER);
  lua_setfield(L, -2, "mininteger");
  setrandfunc(L);
*/