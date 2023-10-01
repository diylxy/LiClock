#include <A_Config.h>
#include "AppManager.h"

bool file_exist(const char *path)
{
    FILE *fp = fopen(getRealPath(path), "r");
    if (fp != NULL)
    {
        fclose(fp);
        return true;
    }
    return false;
}

LuaAppWrapper::LuaAppWrapper(const String filename, const String path)
{
    initialize(filename, path);
}
void LuaAppWrapper::initialize(const String filename, const String path)
{
    this->path = path;
    strncpy(this->_name, filename.c_str(), 36);
    this->name = this->_name;
    isLuaApp = true;
    _showInList = false;
}
LuaAppWrapper::~LuaAppWrapper()
{
}

void LuaAppWrapper::init()
{
    if (file_exist((path + "/conf.lua").c_str()))
    {
        Serial.print("文件存在: ");
        Serial.println(path);
        closeLua();
        openLua_simple();
        lua_pushstring(L, "无标题");
        lua_setglobal(L, "title");
        lua_pushinteger(L, PIN_BUTTONC);
        lua_setglobal(L, "wakeupIO1");
        lua_pushinteger(L, PIN_BUTTONL);
        lua_setglobal(L, "wakeupIO2");
        lua_pushboolean(L, false);
        lua_setglobal(L, "noDefaultEvent");
        lua_pushinteger(L, 0);
        lua_setglobal(L, "peripherals_requested");

        lua_execute((path + "/conf.lua").c_str());
        lua_getglobal(L, "title");
        if (lua_isstring(L, -1))
        {
            strncpy(_title, lua_tostring(L, -1), 32);
            title = _title;
        }
        lua_settop(L, 0);
        lua_getglobal(L, "wakeupIO1");
        if (lua_isinteger(L, -1))
        {
            wakeupIO[0] = lua_tointeger(L, -1);
        }
        lua_settop(L, 0);
        lua_getglobal(L, "wakeupIO2");
        if (lua_isinteger(L, -1))
        {
            wakeupIO[1] = lua_tointeger(L, -1);
        }
        lua_settop(L, 0);
        lua_getglobal(L, "noDefaultEvent");
        if (lua_isboolean(L, -1))
        {
            noDefaultEvent = lua_toboolean(L, -1);
        }
        lua_settop(L, 0);
        lua_getglobal(L, "peripherals_requested");
        if (lua_isinteger(L, -1))
        {
            peripherals_requested = lua_tointeger(L, -1);
        }
        lua_settop(L, 0);
        if (file_exist((path + "/icon.lbm").c_str()))
        {
            FILE *fp = fopen((path + "/icon.lbm").c_str(), "rb");
            fseek(fp, 4, SEEK_SET);
            fread(_image, 128, 1, fp);
            fclose(fp);
            image = _image;
        }
        _showInList = true;
        closeLua();
    }
}
static void luawrapper_lightsleep()
{
    lua_getglobal(L, "lightsleep");
    if (lua_isfunction(L, -1))
    {
        lua_call(L, 0, 0);
    }
}
static void luawrapper_wakeup()
{
    lua_getglobal(L, "wakeup");
    if (lua_isfunction(L, -1))
    {
        lua_call(L, 0, 0);
    }
}
static void luawrapper_exit()
{
    lua_getglobal(L, "exit");
    if (lua_isfunction(L, -1))
    {
        lua_call(L, 0, 0);
    }
}
static void luawrapper_deepsleep()
{
    lua_getglobal(L, "deepsleep");
    if (lua_isfunction(L, -1))
    {
        lua_call(L, 0, 0);
    }
}
void LuaAppWrapper::setup()
{
    setPath(path.c_str());
    closeLua();
    openLua();
    Serial.println("正在打开Lua APP");
    Serial.printf("getRealPath: %s\n", getRealPath("./main.lua"));
    if (file_exist(getRealPath("./main.lua")))
    {
        lua_execute("./main.lua");
        lua_getglobal(L, "lightsleep");
        if (lua_isfunction(L, -1))
        {
            lightsleep = luawrapper_lightsleep;
        }
        else
        {
            lightsleep = NULL;
        }
        lua_settop(L, 0);
        lua_getglobal(L, "wakeup");
        if (lua_isfunction(L, -1))
        {
            wakeup = luawrapper_wakeup;
        }
        else
        {
            wakeup = NULL;
        }
        lua_settop(L, 0);
        lua_getglobal(L, "exit");
        if (lua_isfunction(L, -1))
        {
            exit = luawrapper_exit;
        }
        else
        {
            exit = NULL;
        }
        lua_settop(L, 0);
        lua_getglobal(L, "deepsleep");
        if (lua_isfunction(L, -1))
        {
            deepsleep = luawrapper_deepsleep;
        }
        else
        {
            deepsleep = NULL;
        }
        lua_settop(L, 0);
        lua_getglobal(L, "setup");
        if (lua_isfunction(L, -1))
        {
            lua_call(L, 0, 0);
        }
    }
}

void newLuaApp(const String filename, const String path)
{
    if (!file_exist((path + "/conf.lua").c_str()))
    {
        return;
    }
    if (!file_exist((path + "/main.lua").c_str()))
    {
        return;
    }
    // Serial.printf("[文件] 新的LuaApp: %s (%s)\n", filename.c_str(), path.c_str());
    LuaAppWrapper *ptr = new LuaAppWrapper(filename, path);
    ptr->init();
}

void searchForLuaAPP()
{
    File root, file;
    root = LittleFS.open("/");
    if (!root)
    {
        Serial.println("[文件] root未打开");
    }
    file = root.openNextFile();
    while (file)
    {
        String name = file.name();
        if (file.isDirectory())
        {
            if (name.endsWith(".app"))
            {
                newLuaApp(name, "/littlefs/" + name);
            }
        }
        file.close();
        file = root.openNextFile();
    }
    root.close();
}
