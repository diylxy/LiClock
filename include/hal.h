#ifndef __HAL_H__
#define __HAL_H__
#include <A_Config.h>
#include <Preferences.h>
#include "OneButton.h"
class HAL
{
public:
    void saveConfig();
    void loadConfig();
    void getTime();
    void WiFiConfigSmartConfig();
    void WiFiConfigManual();
    void ReqWiFiConfig();
    /**
     * @brief 初始化
     * @return true 需要全屏刷新
     * @return false 不需要全屏刷新
     */
    bool init();
    void autoConnectWiFi();
    void powerOff(bool displayMessage = true);
    void goSleep(uint32_t sec = 0);
    void update();
    int getNTPMinute();
    void checkNightSleep();
    void setWakeupIO(int io1, int io2);
    void copy(File &newFile, File &file);
    void rm_rf(const char *path);
    struct tm timeinfo;
    time_t now;
    int global_hour_offset = 0;
    time_t lastsync = 1;
    int32_t every = 100;
    int32_t delta = 0;
    int32_t upint = 2 * 60;                 // NTP同步间隔
    int32_t last_update_delta = 0x7FFFFFFF; // 上次更新时修正时间与实际时间的差值
    Preferences pref;
    int16_t VCC = 0;
    bool USBPluggedIn = false;
    bool isCharging = false;
    OneButton btnr = OneButton(PIN_BUTTONR);
    OneButton btnl = OneButton(PIN_BUTTONL);
    OneButton btnc = OneButton(PIN_BUTTONC);
    bool btn_activelow = true;
    void hookButton()
    {
        _hookButton = true;
        while (btnr.isPressing() || btnl.isPressing() || btnc.isPressing())
        {
            delay(10);
        }
        delay(10);
        Serial.println("Hooked");
    }
    void unhookButton()
    {
        while (btnr.isPressing() || btnl.isPressing() || btnc.isPressing())
        {
            delay(10);
        }
        delay(10);
        _hookButton = false;
        Serial.println("Unhooked");
    }
    void detachAllButtonEvents()
    {
        btnr.attachClick(NULL);
        btnr.attachDoubleClick(NULL);
        btnr.attachLongPressStart(NULL);
        btnr.attachDuringLongPress(NULL);
        btnr.attachLongPressStop(NULL);
        btnr.attachMultiClick(NULL);
        btnr.attachClick(NULL, NULL);
        btnr.attachDoubleClick(NULL, NULL);
        btnr.attachLongPressStart(NULL, NULL);
        btnr.attachDuringLongPress(NULL, NULL);
        btnr.attachLongPressStop(NULL, NULL);
        btnr.attachMultiClick(NULL, NULL);
        
        btnl.attachClick(NULL);
        btnl.attachDoubleClick(NULL);
        btnl.attachDuringLongPress(NULL);
        btnl.attachLongPressStop(NULL);
        btnl.attachMultiClick(NULL);
        btnl.attachClick(NULL, NULL);
        btnl.attachDoubleClick(NULL, NULL);
        btnl.attachLongPressStart(NULL, NULL);
        btnl.attachDuringLongPress(NULL, NULL);
        btnl.attachLongPressStop(NULL, NULL);
        btnl.attachMultiClick(NULL, NULL);

        btnc.attachClick(NULL);
        btnc.attachDoubleClick(NULL);
        btnc.attachLongPressStart(NULL);
        btnc.attachDuringLongPress(NULL);
        btnc.attachLongPressStop(NULL);
        btnc.attachMultiClick(NULL);
        btnc.attachClick(NULL, NULL);
        btnc.attachDoubleClick(NULL, NULL);
        btnc.attachLongPressStart(NULL, NULL);
        btnc.attachDuringLongPress(NULL, NULL);
        btnc.attachLongPressStop(NULL, NULL);
        btnc.attachMultiClick(NULL, NULL);
    }
    bool noDeepSleep = false;
    bool SleepUpdateMutex = false;
    bool _hookButton = false; // 不要修改这个
    bool wakeUpFromDeepSleep = false;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int _wakeupIO[2] = {PIN_BUTTONC, PIN_BUTTONL};

private:
};
extern HAL hal;
#endif