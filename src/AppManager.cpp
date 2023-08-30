#include <AppManager.h>
AppManager appManager;
#define MAX_APP_COUNT 128
AppBase *appList[MAX_APP_COUNT];
int tail = 0;
char RTC_DATA_ATTR latest_appname[36] = "";
void appList_push_back(AppBase *app)
{
    appList[tail++] = app;
}

AppBase::AppBase()
{
    lightsleep = NULL;
    wakeup = NULL;
    exit = NULL;
    deepsleep = NULL;
    appID = appManager.getAValidAppID();
    appManager.increaseValidAppID();
    appList_push_back(this);
}
AppBase::~AppBase()
{
}

AppBase *AppManager::getPtrByName(const char *appName)
{
    for (int16_t i = 0; i < tail; i++)
    {
        if (strcmp(appList[i]->name, appName) == 0)
            return appList[i];
    }
    return NULL;
}

void AppManager::gotoApp(AppBase *appPtr)
{
    AppBase *app_to = NULL;
    app_to = appPtr;
    if (app_to == NULL)
        return;
    this->app_to = app_to;
    method = APPMANAGER_GOTOAPP;
}

void AppManager::goBack()
{
    if (appStack.empty() == true)
    {
        return;
    }
    method = APPMANAGER_GOBACK;
}
extern const uint8_t defaultAppIcon[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x00,
    0x00, 0x03, 0x80, 0x01, 0x00, 0x05, 0x40, 0x01, 0x00, 0x09, 0x20, 0x01,
    0x00, 0x11, 0x10, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x81, 0x03, 0x01, 0x00, 0x41, 0x04, 0x01, 0x00, 0x01, 0x04, 0x01,
    0x00, 0x01, 0x02, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x10, 0x01, 0x00, 0x09, 0x20, 0x01,
    0x00, 0x05, 0x40, 0x01, 0x00, 0x03, 0x80, 0x01, 0x00, 0xfe, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t goBackIcon[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00,
    0x00, 0xb0, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x00,
    0x00, 0x86, 0x00, 0x00, 0x00, 0x83, 0xff, 0x07, 0x80, 0x81, 0x00, 0x08,
    0xc0, 0x80, 0x00, 0x10, 0x60, 0x80, 0x00, 0x10, 0x30, 0x80, 0x00, 0x10,
    0x18, 0x80, 0x00, 0x10, 0x30, 0x81, 0x00, 0x14, 0x60, 0x82, 0x00, 0x14,
    0xc0, 0x84, 0xf8, 0x13, 0x80, 0x89, 0x00, 0x08, 0x00, 0xb3, 0xff, 0x07,
    0x00, 0x86, 0x00, 0x00, 0x00, 0x8c, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00,
    0x00, 0xb0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const uint8_t wifiIcon[] = {
    0x00, 0x00, 0xf0, 0x0f, 0xfc, 0x3f, 0x1e, 0x78, 0x07, 0xe0, 0xe0, 0x07,
    0xf8, 0x1f, 0x38, 0x1c, 0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0x80, 0x01,
    0x00, 0x00};
static AppBase *realAppList[MAX_APP_COUNT];
static int realAppCount = 0;
void AppManager::showAppList()
{
    // 下面是标题部分
    {
        char buf[30];
        display.drawRoundRect(0, 0, 296, 128, 3, 0);
        // 标题栏
        display.drawFastHLine(0, 16, 296, 0);
        u8g2Fonts.setBackgroundColor(1);
        u8g2Fonts.setForegroundColor(0);
        sprintf(buf, "%02d:%02d", hal.timeinfo.tm_hour, hal.timeinfo.tm_min);
        u8g2Fonts.drawUTF8(2, 12, buf);
        // 右侧状态图标
        int16_t x = 294;
        // 电池
        display.drawXBitmap(x - 20, 0, getBatteryIcon(), 20, 16, 0);
        x -= 20 - 2;
        // 电池电量数值
        u8g2Fonts.setCursor(x - 44, 12);
        u8g2Fonts.printf("%d.%03dV", hal.VCC / 1000, hal.VCC % 1000);
        x -= 50 - 4;
        // WiFi
        if (WiFi.isConnected())
        {
            display.drawXBitmap(x - 16, 2, wifiIcon, 16, 13, 0);
            x -= 16 - 2;
        }
    }
    // 返回按钮
    display.drawXBitmap(12, 21, goBackIcon, 32, 32, 0);
    u8g2Fonts.drawUTF8(16, 65, "返回");
    int count = 1;
    for (int16_t i = 0; i < tail; i++)
    {
        if (appList[i]->_showInList == false)
        {
            continue;
        }
        if (peripherals.checkAvailable(appList[i]->peripherals_requested) != 0)
        {
            continue;
        }
        realAppList[count - 1] = appList[i];
        int16_t x, y;
        x = (count / 2) * 49 + 4;
        y = (count % 2) * 52 + 21; // App左上角位置
        if (appList[i]->image != NULL)
        {
            display.drawXBitmap(x + 8, y, appList[i]->image, 32, 32, 0);
        }
        else
        {
            display.drawXBitmap(x + 8, y, defaultAppIcon, 32, 32, 0);
        }
        int w = u8g2Fonts.getUTF8Width(appList[i]->title);
        int x_font_offset = 0;
        if (w <= 48)
        {
            x_font_offset = (48 - w) / 2;
        }
        u8g2Fonts.drawUTF8(x + x_font_offset, y + 45, appList[i]->title);
        count++;
    }
    realAppCount = count - 1;
}
AppBase *AppManager::appSelector()
{
    display.swapBuffer(1);
    display.clearScreen();
    showAppList();
    display.drawRoundRect(4 - 1, 21 - 2, 50, 50, 5, 0); // 绘制选择框
    display.display(true);
    // 下面是选择
    int selected = 0;
    hal.hookButton();
    int last_selected = 0;
    int idleTime = 0;
    bool waitc = false;
    while (1)
    {
        if (digitalRead(PIN_BUTTONL) == 0)
        {
            idleTime = 0;
            selected--;
            if (selected < 0)
                selected = realAppCount; // 这里没问题，不要改
        }
        if (digitalRead(PIN_BUTTONR) == 0)
        {
            idleTime = 0;
            selected++;
            if (selected > realAppCount) // 这里也没问题
                selected = 0;
        }
        if (digitalRead(PIN_BUTTONC) == LOW)
        {
            delay(20);
            if (digitalRead(PIN_BUTTONC) == LOW)
            {
                if (GUI::waitLongPress(PIN_BUTTONC) == true)
                {
                    selected = 0;
                    waitc = true;
                }
                else
                {
                    break;
                }
            }
        }
        if (selected != last_selected)
        {
            int16_t x, y;
            int16_t last_x, last_y;
            x = (selected / 2) * 49 + 4;
            y = (selected % 2) * 52 + 21; // App左上角位置
            if (last_selected == -1)
                last_selected = 0;
            last_x = (last_selected / 2) * 49 + 4;
            last_y = (last_selected % 2) * 52 + 21;
            last_selected = selected;
            display.drawRoundRect(last_x - 1, last_y - 2, 50, 50, 5, 1); // 清除上一个选择框
            display.drawRoundRect(x - 1, y - 2, 50, 50, 5, 0);           // 绘制选择框
            display.display(true);
        }
        if (waitc == true)
        {
            waitc = false;
            while (digitalRead(PIN_BUTTONC) == LOW)
                delay(10);
            delay(10);
        }
        delay(10);
        idleTime++;
        if (idleTime > 6000)
        {
            // 60s无操作，自动返回
            selected = 0;
            break;
        }
    }
    hal.unhookButton();
    if (selected == 0)
    {
        display.swapBuffer(0);
        display.display(false);
    }
    else
    {
        display.clearScreen();
        display.setCursor(60, 72);
        display.setFont(&FreeSans18pt7b);
        display.print("Loading...");
        display.display(true);
        display.swapBuffer(0);
    }
    if (selected == 0)
        return NULL;
    return realAppList[selected - 1]; // 这里没问题
}

void AppManager::update()
{
    static bool updateAgain = false;
    if (currentApp == NULL && app_to == NULL)
        return; // App还未加载，直接返回
    // 判断是否需要进入App选择界面
    if (method == APPMANAGER_SHOWAPPSELECTOR)
    {
        method = APPMANAGER_NOOPERATION;
        AppBase *res = appSelector();
        if (res != NULL)
        {
            this->app_to = res;
            method = APPMANAGER_GOTOAPP;
            Serial.printf("正在跳转到APP：%d:%s\n", app_to->appID, app_to->name);
            return;
        }
        updateAgain = true;
    }
    else if (method == APPMANAGER_GOTOAPP)
    {
        method = APPMANAGER_NOOPERATION;
        // 注意只能有一个App占有屏幕，所以在切换App时先退出上个App
        if (currentApp != NULL)
        {
            if (currentApp->exit != NULL)
            {
                currentApp->exit();
            }
        }
        attachLocalEvent();
        if (app_to != currentApp && currentApp != NULL)
        {
            appStack.push(currentApp);
        }
        fTimer = NULL;
        timer_interval = 0;
        nextWakeup = 0;
        noDeepSleep = false;
        currentApp = app_to;
        strncpy(latest_appname, app_to->name, 36);
        hal.setWakeupIO(currentApp->wakeupIO[0], currentApp->wakeupIO[1]);
        if (currentApp->noDefaultEvent)
            hal.detachAllButtonEvents();
        if (peripherals.load(currentApp->peripherals_requested) == false)
        {
            GUI::msgbox("错误", "外设加载失败，APP运行将不稳定");
        }
        currentApp->setup();
        parameter = "";
        updateAgain = true;
    }
    else if (method == APPMANAGER_GOBACK)
    {
        method = APPMANAGER_NOOPERATION;
        if (appStack.size() == 0)
            return;
        // goback时：currentApp就是当前前台App，appStack未变化
        // 首先执行app退出
        if (currentApp->exit != NULL)
            currentApp->exit();
        // 然后准备环境
        currentApp = appStack.top();
        appStack.pop();
        strncpy(latest_appname, currentApp->name, 36);
        fTimer = NULL;
        timer_interval = 0;
        nextWakeup = 0;
        noDeepSleep = false;
        // 然后执行前一app初始化
        hal.setWakeupIO(currentApp->wakeupIO[0], currentApp->wakeupIO[1]);
        if (currentApp->noDefaultEvent)
            hal.detachAllButtonEvents();
        if (peripherals.load(currentApp->peripherals_requested) == false)
        {
            GUI::msgbox("错误", "外设加载失败，APP运行将不稳定");
        }
        currentApp->setup();
        updateAgain = true;
    }
    else
    {
        method = APPMANAGER_NOOPERATION;
    }
    // 判断是否需要执行定时器
    if (fTimer != NULL && hal.now >= timer_triggertime)
    {
        fTimer();
        timer_triggertime = hal.now + timer_interval;
        updateAgain = true;
    }
    if (updateAgain)
    {
        updateAgain = false;
        return;
    }
    if (digitalRead(PIN_BUTTONL) == 1 && digitalRead(PIN_BUTTONR) == 1 && digitalRead(PIN_BUTTONC) == 1 && hal.btnl.isIdle() == true && hal.btnr.isIdle() == true && hal.btnc.isIdle() == true)
    {
        // 准备进入睡眠模式
        // 计算下一次唤醒时间
        int realNextWakeup = 0;
        if (nextWakeup != 0)
        {
            if (nextWakeup > timer_triggertime && fTimer != NULL && timer_triggertime != 0 && timer_interval != 0)
            {
                realNextWakeup = timer_triggertime - hal.now;
            }
            else
            {
                realNextWakeup = nextWakeup;
            }
        }
        else if (fTimer != NULL && timer_triggertime != 0 && timer_interval != 0)
        {
            realNextWakeup = timer_interval;
        }
        else
        {
            realNextWakeup = 0;
        }

        hal.noDeepSleep = noDeepSleep;
        if (noDeepSleep == true)
        {
            if (currentApp->lightsleep != NULL)
            {
                currentApp->lightsleep();
            }
        }
        else
        {
            if (currentApp->deepsleep != NULL)
            {
                currentApp->deepsleep();
            }
            else if (currentApp->exit != NULL)
            {
                currentApp->exit();
            }
        }
        // 等待锁
        while (hal.SleepUpdateMutex)
            delay(2);
        hal.SleepUpdateMutex = true;
        if (realNextWakeup != 0)
        {
            hal.goSleep(realNextWakeup);
        }
        else
        {
            hal.powerOff(false);
        }
        hal.SleepUpdateMutex = false;
        method = APPMANAGER_NOOPERATION;
        if (currentApp->wakeup != NULL)
        {
            currentApp->wakeup();
        }
    }
}

void AppManager::setTimer(uint32_t second, void (*fn)())
{
    if (second == 0 || fn == NULL)
        return;
    fTimer = fn;
    timer_interval = second;
    timer_triggertime = hal.now + second;
}

void AppManager::clearTimer()
{
    fTimer = NULL;
    timer_interval = 0;
}

void AppManager::attachLocalEvent()
{
    hal.detachAllButtonEvents();
    hal.btnc.attachLongPressStart([](void *scope)
                                  {if( ((AppManager *)scope)->currentApp->noDefaultEvent == false) ((AppManager *)scope)->method = APPMANAGER_SHOWAPPSELECTOR; },
                                  this);
    hal.btnl.attachLongPressStart([](void *scope)
                                  { if( ((AppManager *)scope)->currentApp->noDefaultEvent == false) {((AppManager *)scope)->method = APPMANAGER_GOBACK; Serial.println("Back."); } },
                                  this);
}
void AppManager::gotoAppBoot(const char *appName)
{
    appStack.push(getRealClock());
    gotoApp(appName);
}

bool AppManager::recover()
{
    if (latest_appname[0] != 0)
    {
        Serial.print("重新打开上个APP：");
        Serial.println(latest_appname);
        appStack.push(getRealClock());
        gotoApp(latest_appname);
        return true;
    }
    return false;
}