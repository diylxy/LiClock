#include "A_Config.h"
extern const char *err_invalid_param;

static int gui_waitLongPress(lua_State *L)
{
    if (lua_gettop(L) != 1)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int btn = luaL_checkinteger(L, 1);
    bool ret = GUI::waitLongPress(btn);
    lua_pushboolean(L, ret);
    return 1;
}
static int gui_autoIndentDraw(lua_State *L)
{
    if (lua_gettop(L) != 3 && lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    const char *str = luaL_checkstring(L, 1);
    int max_x = lua_tointeger(L, 2);
    if (max_x <= 0)
    {
        max_x = 296;
    }
    int start_x = 2;
    if (lua_gettop(L) == 3)
    {
        start_x = luaL_checkinteger(L, 3);
    }
    GUI::autoIndentDraw(str, max_x, start_x);
    return 0;
}
static int gui_drawWindowsWithTitle(lua_State *L)
{
    const char *title = NULL;
    int16_t x = 0, y = 0, w, h;
    title = luaL_checkstring(L, 1);
    x = lua_tointeger(L, 2);
    y = lua_tointeger(L, 3);
    w = lua_tointeger(L, 4);
    h = lua_tointeger(L, 5);
    if (w <= 0)
        w = 296;
    if (h <= 0)
        h = 128;
    GUI::drawWindowsWithTitle(title, x, y, w, h);
    return 0;
}

static int gui_msgbox(lua_State *L)
{
    if (lua_gettop(L) == 0)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    const char *title = luaL_checkstring(L, 1);
    const char *msg = lua_tostring(L, 2);
    GUI::msgbox(title, msg);
    return 0;
}

static int gui_msgbox_yn(lua_State *L)
{
    const char *title = NULL;
    const char *msg = NULL;
    const char *yes = NULL;
    const char *no = NULL;
    title = luaL_checkstring(L, 1);
    msg = lua_tostring(L, 2);
    yes = lua_tostring(L, 3);
    no = lua_tostring(L, 4);
    bool ret = GUI::msgbox_yn(title, msg, yes, no);
    lua_pushboolean(L, ret);
    return 1;
}

static int gui_msgbox_number(lua_State *L)
{
    const char *title = NULL;
    uint16_t digits = 0;
    int pre_value = 0;
    title = luaL_checkstring(L, 1);
    digits = lua_tointeger(L, 2);
    if (digits == 0)
        digits = 1;
    pre_value = lua_tointeger(L, 3);
    int ret = GUI::msgbox_number(title, digits, pre_value);
    lua_pushinteger(L, ret);
    return 1;
}

static int gui_menu(lua_State *L)
{
    const char *title = NULL;
    title = luaL_checkstring(L, 1);
    int len = lua_rawlen(L, 2); // 第二个参数：table，里面是string
    Serial.print("len=");
    Serial.println(len);
    menu_item *options = new menu_item[len + 1];
    for (int i = 0; i < len; i++)
    {
        // 现在table在栈顶
        lua_pushinteger(L, i + 1);              // 压入位置
        lua_gettable(L, -2);                    // 总表在-2，栈顶是当前菜单项
        options[i].title = new char[40];
        strncpy((char *)options[i].title, lua_tostring(L, -1), 40); // 读取标题
        options[i].icon = NULL;
        lua_pop(L, 1);
    }
    options[len].title = NULL;
    options[len].icon = NULL;
    int ret = GUI::menu(title, options);
    for (int i = 0; i < len; i++)
    {
        delete[] options[i].title;
    }
    delete[] options;
    lua_pushinteger(L, ret + 1);
    return 1;
}

static int gui_drawLBM(lua_State *L)
{
    if (lua_gettop(L) != 4)
    {
        lua_pushstring(L, err_invalid_param);
        lua_error(L);
        return 0;
    }
    int16_t x = luaL_checkinteger(L, 1);
    int16_t y = luaL_checkinteger(L, 2);
    const char *filename = luaL_checkstring(L, 3);
    uint16_t color = luaL_checkinteger(L, 4);
    GUI::drawLBM(x, y, filename, color);
    return 0;
}

static int gui_fileDialog(lua_State *L)
{
    const char *title = NULL;
    title = luaL_checkstring(L, 1);
    const char *ret = GUI::fileDialog(title);
    lua_pushstring(L, ret);
    return 1;
}

static const luaL_Reg _lualib[] = {
    {"waitLongPress", gui_waitLongPress},
    {"autoIndentDraw", gui_autoIndentDraw},
    {"drawWindowsWithTitle", gui_drawWindowsWithTitle},
    {"msgbox", gui_msgbox},
    {"msgbox_yn", gui_msgbox_yn},
    {"msgbox_number", gui_msgbox_number},
    {"menu", gui_menu},
    {"drawLBM", gui_drawLBM},
    {"fileDialog", gui_fileDialog},
    {NULL, NULL},
};

extern "C" int luaopen_gui(lua_State *L)
{
    luaL_newlib(L, _lualib);
    return 1;
}