#pragma once
#include <A_Config.h>
#include <list>
#include <stack>

class AppBase
{
private:
public:
    const char *name = "BaseApp";        // 一个标识符，唯一标识这个App
    const char *title = "BaseApp";       // 一个标题，显示在App列表中
    const char *description = "BaseApp"; // App描述
    const uint8_t *image = NULL;         // App图像，32*32,xbm格式
    int appID = 0;                       // AppID，唯一标识App
    bool _showInList = true;
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
     * @brief 进入deepsleep前执行
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
    int getIDByName(const char *appName);
    void gotoApp(int appID);
    void gotoApp(const char *appName)
    {
        int appID = getIDByName(appName);
        gotoApp(appID);
    }
    void gotoDefaultApp()
    {
        gotoApp("clock");
    }
    void goBack();
    void showAppList(); // 显示Applist
    int appSelector();  // 显示Applist并等待用户输入
    void update();
    String parameter = "";                        // 传递的参数，会在goto目标app的setup执行完后自动清空
    String result = "";                           // 传递的返回值，不会自动清空
    void setTimer(uint32_t second, void (*fn)()); // 设置周期执行函数，如果存在timer，则只进入lightsleep
    void clearTimer();                            // 清除定时器
    bool noDeepSleep = false;                     // 禁止进入DeepSleep模式
    uint32_t nextWakeup = 0;                      // 预定下次唤醒，注意如果是deepsleep唤醒需要重新设置
    void attachLocalEvent();                      // 附加appManager按钮事件
};

extern AppManager appManager;
extern AppBase *appList[128];