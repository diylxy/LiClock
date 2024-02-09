#pragma once
#include <A_Config.h>
#include <list>
#include <stack>

class AppBase
{
private:
public:
    const char *name = "BaseApp";                 // 一个标识符，唯一标识这个App
    const char *title = "BaseApp";                // 一个标题，显示在App列表中
    const char *description = "BaseApp";          // App描述
    const uint8_t *image = NULL;                  // App图像，32*32,xbm格式
    int appID = 0;                                // AppID，唯一标识App
    bool _showInList = true;                      // 是否展示在App列表
    int wakeupIO[2] = {PIN_BUTTONC, PIN_BUTTONL}; // 唤醒IO
    bool noDefaultEvent = false;                  // 禁用默认按钮事件
    uint16_t peripherals_requested = 0;           // 请求的外设
    bool isLuaApp = false;                        // 是否是LuaApp
    bool _reentrant = true;                       // 阻止休眠后重新打开，用于某些只能运行一次的App或临时App对象

    /**
     * @brief 初始化(App打开)
     */
    virtual void setup(){};
    /**
     * @brief 进入lightsleep前执行
     */
    void (*lightsleep)();
    /**
     * @brief lightsleep唤醒后执行
     */
    void (*wakeup)();
    /**
     * @brief App退出时执行
     */
    void (*exit)();
    /**
     * @brief 进入deepsleep前执行，注意如果没有此函数，会尝试执行exit函数
     */
    void (*deepsleep)();
    AppBase();
    ~AppBase();
};

class AppManager
{
private:
    /* data */
    std::stack<AppBase *> appStack;
    enum
    {
        APPMANAGER_NOOPERATION = 0,
        APPMANAGER_GOBACK = 1,
        APPMANAGER_GOTOAPP = 2,
        APPMANAGER_SHOWAPPSELECTOR = 3,
    } method;
    int validAppID = 1;
    void (*fTimer)() = NULL;
    uint32_t timer_interval = 0;
    time_t timer_triggertime = 0;

public:
    AppBase *currentApp = NULL;
    AppBase *app_to = NULL;
    int getAValidAppID()
    {
        return validAppID;
    }
    void increaseValidAppID()
    {
        validAppID++;
    }
    AppBase *getPtrByName(const char *appName);
    AppBase *getRealClock();
    void gotoApp(AppBase *appPtr);
    void gotoApp(const char *appName);
    void gotoAppBoot(const char *appName);
    bool recover(AppBase *home = NULL);
    void goBack();
    void showAppList(int page);                    // 显示Applist
    AppBase *appSelector(bool showHidden = false); // 显示Applist并等待用户输入
    void update();
    String parameter = "";                        // 传递的参数，会在goto目标app的setup执行完后自动清空
    String result = "";                           // 传递的返回值，不会自动清空
    void setTimer(uint32_t second, void (*fn)()); // 设置周期执行函数，如果存在timer，则只进入lightsleep
    void clearTimer();                            // 清除定时器
    bool noDeepSleep = false;                     // 禁止进入DeepSleep模式
    uint32_t nextWakeup = 0;                      // 预定下次唤醒，注意如果是deepsleep唤醒需要重新设置
    void attachLocalEvent();                      // 附加appManager按钮事件
    String bootapp;
    bool luaLoaded = false;
    void loadLuaApps();
};

extern AppManager appManager;
extern AppBase *appList[128];

class LuaAppWrapper : public AppBase
{
private:
    String path;
    char _title[32];
    char _name[36];
    uint8_t _image[128];

public:
    LuaAppWrapper(const String filename, const String path);
    LuaAppWrapper()
    {
        isLuaApp = true;
        _showInList = false;
    };
    void
    initialize(const String filename, const String path);
    ~LuaAppWrapper();
    void init();
    void setup();
};

extern void newLuaApp(const char *filename);
extern void searchForLuaAPP();