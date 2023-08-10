#include <A_Config.h>
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
GxEPD2_BW<GxEPD2_290, GxEPD2_290::HEIGHT> display(GxEPD2_290(/*CS=5*/ CONFIG_SPI_CS, /*DC=*/CONFIG_PIN_DC, /*RST=*/CONFIG_PIN_RST, /*BUSY=*/CONFIG_PIN_BUSY)); // 注意：此类略微修改过，使用两个缓冲区
DynamicJsonDocument config(1024);

void task_appManager(void *)
{
    while (1)
    {
        appManager.attachLocalEvent();
        appManager.update();
    }
}
void setup()
{
    bool initResult = hal.init();
    xTaskCreate(task_appManager, "appManager", 8192, NULL, 1, NULL);
    if (initResult == false)
    {
        appManager.parameter = "p";
    }
    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED)
    {
        //TODO:尝试恢复之前的App
        
    }
    if (config[PARAM_CLOCKONLY] == "1")
    {
        appManager.gotoApp("clockonly");
    }
    else
    {
        appManager.gotoApp("clock");
    }
    return;
}

int NTPCounter = 0;
void loop()
{
    vTaskDelay(portMAX_DELAY);
}
