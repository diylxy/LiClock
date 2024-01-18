#include "AppManager.h"

class AppDemoBMP280 : public AppBase
{
private:
    /* data */
public:
    AppDemoBMP280()
    {
        name = "demobmp280";
        title = "BMP280";
        description = "模板";
        image = NULL;
        peripherals_requested = PERIPHERALS_BMP280_BIT;
        _showInList = false;
    }
    void setup();
};
static AppDemoBMP280 app;

void AppDemoBMP280::setup()
{
    char buf[50];
    xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
    if (peripherals.bmp.takeForcedMeasurement())
    {
        Serial.println(weather.realtime.pressure);
        sprintf(buf, "气压: %g Pa\n温度: %g ℃\n海拔: %g m", peripherals.bmp.readPressure(), peripherals.bmp.readTemperature(), peripherals.bmp.readAltitude(weather.realtime.pressure / 100));
        xSemaphoreGive(peripherals.i2cMutex);
    }
    else
    {
        xSemaphoreGive(peripherals.i2cMutex);
        sprintf(buf, "测量失败");
    }
    Serial.println(buf);
    GUI::msgbox("传感器信息", buf);
    appManager.goBack();
}
