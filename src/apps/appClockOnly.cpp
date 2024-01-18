#include "AppManager.h"
static const uint8_t clock_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0xfe, 0x7f, 0x00,
    0x80, 0x1f, 0xf8, 0x01, 0xc0, 0x03, 0xe0, 0x03, 0xe0, 0x01, 0x80, 0x07,
    0xf0, 0x00, 0x00, 0x0f, 0x78, 0x80, 0x01, 0x1e, 0x38, 0x80, 0x01, 0x1c,
    0x1c, 0x80, 0x01, 0x38, 0x1c, 0x80, 0x01, 0x30, 0x0e, 0x80, 0x01, 0x70,
    0x0e, 0x80, 0x01, 0x70, 0x06, 0x80, 0x01, 0x60, 0x06, 0x80, 0x01, 0x60,
    0x06, 0x80, 0x01, 0x60, 0x06, 0x80, 0x03, 0x60, 0x06, 0x80, 0x07, 0x60,
    0x06, 0x00, 0x0e, 0x60, 0x0e, 0x00, 0x1c, 0x70, 0x0e, 0x00, 0x38, 0x30,
    0x1c, 0x00, 0x10, 0x38, 0x1c, 0x00, 0x00, 0x38, 0x38, 0x00, 0x00, 0x1c,
    0x78, 0x00, 0x00, 0x1e, 0xf0, 0x00, 0x00, 0x0f, 0xe0, 0x01, 0x80, 0x07,
    0xc0, 0x07, 0xe0, 0x03, 0x80, 0x1f, 0xf8, 0x01, 0x00, 0xfe, 0x7f, 0x00,
    0x00, 0xf8, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00};
class AppClockOnly : public AppBase
{
private:
    /* data */
public:
    AppClockOnly()
    {
        name = "clockonly";
        title = "仅时钟";
        description = "仅时钟模式";
        image = clock_bits;
    }
    void setup();
};
static AppClockOnly app;
extern const char *dayOfWeek[];
void AppClockOnly::setup()
{
    int w;
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", hal.timeinfo.tm_hour, hal.timeinfo.tm_min);
    display.clearScreen();
    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setForegroundColor(0);
    u8g2Fonts.setBackgroundColor(1);
    u8g2Fonts.setFont(u8g2_font_logisoso92_tn);
    w = u8g2Fonts.getUTF8Width(timeStr);
    u8g2Fonts.setCursor((296 - w) / 2, 104);
    u8g2Fonts.print(timeStr);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    display.drawFastHLine(0, 110, 296, 0);
    u8g2Fonts.setCursor(10, 125);
    u8g2Fonts.printf("%02d月%02d日 星期%s  ", hal.timeinfo.tm_mon + 1, hal.timeinfo.tm_mday, dayOfWeek[hal.timeinfo.tm_wday]);
    if (peripherals.peripherals_current & PERIPHERALS_AHT20_BIT)
    {
        sensors_event_t humidity, temp;
        peripherals.load_append(PERIPHERALS_AHT20_BIT);
        xSemaphoreTake(peripherals.i2cMutex, portMAX_DELAY);
        peripherals.aht.getEvent(&humidity, &temp);
        xSemaphoreGive(peripherals.i2cMutex);
        u8g2Fonts.printf("温度:%.1f℃ 湿度:%.1f%%", temp.temperature, humidity.relative_humidity);
    }
    // 电池
    display.drawXBitmap(296 - 25, 111, getBatteryIcon(), 20, 16, 0);

    if (force_full_update || part_refresh_count > 20)
    {
        display.display(false);
        force_full_update = false;
        part_refresh_count = 0;
    }
    else
    {
        display.display(true);
        part_refresh_count++;
    }
    appManager.noDeepSleep = false;
    appManager.nextWakeup = 61 - hal.timeinfo.tm_sec;
}
