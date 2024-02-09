#include "AppManager.h"

class AppDemoAHT20 : public AppBase
{
private:
    /* data */
public:
    AppDemoAHT20()
    {
        name = "demoaht20";
        title = "AHT20";
        description = "AHT20示例App";
        image = NULL;
        peripherals_requested = PERIPHERALS_AHT20_BIT;
        _showInList = false;
    }
    void setup();
};
static AppDemoAHT20 app;

void AppDemoAHT20::setup()
{
    sensors_event_t humidity, temp;
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    peripherals.aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    xSemaphoreGive(peripherals.i2cMutex);
    char buf[30];
    sprintf(buf, "温度: %g ℃\n相对湿度：%g% rH\n", temp.temperature,humidity.relative_humidity);
    Serial.println(buf);
    GUI::msgbox("传感器信息", buf);
    appManager.goBack();
}
