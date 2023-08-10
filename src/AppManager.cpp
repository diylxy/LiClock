#include <AppManager.h>
AppManager appManager;
AppBase *appList[128];
int tail = 0;
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

int AppManager::getIDByName(const char *appName)
{
    for (int16_t i = 0; i < tail; i++)
    {
        if (strcmp(appList[i]->name, appName) == 0)
            return appList[i]->appID;
    }
    return -1;
}

void AppManager::gotoApp(int appID)
{
    AppBase *app_to = NULL;
    for (int16_t i = 0; i < tail; i++)
    {
        if (appList[i]->appID == appID)
        {
            app_to = appList[i];
            break;
        }
    }
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
}
int AppManager::appSelector()
{
    display.swapBuffer(1);
    display.clearScreen();
    showAppList();
    // 下面是选择
    int selected = 0;
    bool btnaClicked = false;
    bool btnbClicked = false;
    bool btnaLongPressed = false;
    bool btnbLongPressed = false;
    hal.detachAllButtonEvents();
    hal.btna.attachClick([](void *scope)
                         { *((bool *)scope) = true; },
                         &btnaClicked);
    hal.btnb.attachClick([](void *scope)
                         { *((bool *)scope) = true; },
                         &btnbClicked);
    hal.btna.attachLongPressStart([](void *scope)
                                  { *((bool *)scope) = true; },
                                  &btnaLongPressed);
    hal.btnb.attachLongPressStart([](void *scope)
                                  { *((bool *)scope) = true; },
                                  &btnbLongPressed);
    int last_selected = -1;
    int idleTime = 0;
    while (1)
    {
        if (btnbClicked == true)
        {
            idleTime = 0;
            btnbClicked = false;
            selected--;
            if (selected < 0)
                selected = tail; // 这里没问题
        }
        else if (btnaClicked == true)
        {
            idleTime = 0;
            btnaClicked = false;
            selected++;
            if (selected > tail) // 这里也没问题
                selected = 0;
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
        if (btnaLongPressed == true)
        {
            btnaLongPressed = false;
            break;
        }
        else if (btnbLongPressed == true)
        {
            btnbLongPressed = false;
            selected = 0;
            break;
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
    attachLocalEvent();
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
    return selected;
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
        int res = appSelector();
        if (res != 0)
        {
            this->app_to = appList[res - 1];
            method = APPMANAGER_GOTOAPP;
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
        attachLocalEvent();
        appStack.pop();
        currentApp = appStack.top();
        fTimer = NULL;
        timer_interval = 0;
        nextWakeup = 0;
        noDeepSleep = false;
        // 然后执行前一app初始化
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
    if (hal.btna.isIdle() && hal.btnb.isIdle() && digitalRead(PIN_BUTTON) == 1 && digitalRead(PIN_BUTTONB) == 1)
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
    hal.btna.attachLongPressStart([](void *scope)
                                  { ((AppManager *)scope)->method = APPMANAGER_SHOWAPPSELECTOR; },
                                  this);
    hal.btnb.attachLongPressStart([](void *scope)
                                  { ((AppManager *)scope)->method = APPMANAGER_GOBACK; },
                                  this);
}
