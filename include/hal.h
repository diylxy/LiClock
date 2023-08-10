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
    void WiFiConfigManual();
    void ReqWiFiConfig();
    /**
     * @brief 初始化
     * @return true 需要全屏刷新
     * @return false 不需要全屏刷新
     */
    bool init();
    void autoConnectWiFi();
    void drawPageWeatherFrame();
    void drawPageWeatherTime();
    void powerOff(bool displayMessage = true);
    void goSleep(uint32_t sec = 0);
    void update();
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
    OneButton btna = OneButton(PIN_BUTTON);
    OneButton btnb = OneButton(PIN_BUTTONB);
    bool hookButton = false;
    void detachAllButtonEvents()
    {
        btna.attachClick(NULL);
        btna.attachDoubleClick(NULL);
        btna.attachDuringLongPress(NULL);
        btna.attachLongPressStop(NULL);
        btna.attachMultiClick(NULL);
        btnb.attachClick(NULL);
        btnb.attachDoubleClick(NULL);
        btnb.attachDuringLongPress(NULL);
        btnb.attachLongPressStop(NULL);
        btnb.attachMultiClick(NULL);
    }
    bool noDeepSleep = false;
    bool SleepUpdateMutex = false;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
private:
};
extern HAL hal;
#endif