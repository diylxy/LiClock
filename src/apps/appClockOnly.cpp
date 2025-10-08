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
    int extractTimezoneInfo(char *buffer, size_t bufferSize);
    String getSecondaryTimeZone();
    String getSecondaryTime();
};

static AppClockOnly app;
extern const char *dayOfWeek[];
void AppClockOnly::setup()
{
    int x, y;
    int wColon, wHour, wMinu;
    char hourStr[3];
    char minuStr[3];
    bool bSecondaryTZ = strlen(config[PARAM_SECONDARY_TZ]) > 0;
    Serial.printf("P10: %s\n", config[PARAM_SECONDARY_TZ].as<const char*>());
    sprintf(hourStr, "%02d", hal.timeinfo.tm_hour);
    sprintf(minuStr, "%02d", hal.timeinfo.tm_min);
    display.clearScreen();
    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setForegroundColor(0);
    u8g2Fonts.setBackgroundColor(1);
    u8g2Fonts.setFont(u8g2_font_logisoso92_tn);
    wColon = u8g2Fonts.getUTF8Width(":");
    wHour = u8g2Fonts.getUTF8Width(hourStr);
    if (bSecondaryTZ) {
        y = 95;
    }
    else {
        y = 104;
    }
    x = 296/2 - (wHour + wColon / 2); // half screen width - half text width
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(hourStr);
    x += wHour; // move cursor to the end of hour text
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(":");
    if (bSecondaryTZ) {
        u8g2Fonts.setCursor(10, y + 12);
        u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
        String secondaryTime = getSecondaryTime();
        Serial.printf("Secondary time zone: %s\n", secondaryTime.c_str());
        u8g2Fonts.printf(secondaryTime.c_str());
        u8g2Fonts.setFont(u8g2_font_logisoso92_tn);
    }
    wMinu = u8g2Fonts.getUTF8Width(minuStr);
    x += wColon; // move cursor to the end of colon
    u8g2Fonts.setCursor(x, y);
    u8g2Fonts.print(minuStr);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    display.drawFastHLine(0, 110, 296, 0);
    u8g2Fonts.setCursor(10, 127);
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

    // draw secondary time

    if (!force_full_update) {
        // force full update every hour
        force_full_update = (hal.timeinfo.tm_min == 0);
    }
    if (force_full_update || part_refresh_count >= 60)
    {
        Serial.printf("force full update:%d\npart_refresh_count:%d\n", force_full_update, part_refresh_count);
        display.display(false);
        force_full_update = false;
        part_refresh_count = 0;
    }
    else
    {
        Serial.printf("force full update:%d\npart_refresh_count:%d\n", force_full_update, part_refresh_count);
        display.display(true);
        part_refresh_count++;
    }
    appManager.noDeepSleep = false;
    appManager.nextWakeup = 61 - hal.timeinfo.tm_sec;
}

int AppClockOnly::extractTimezoneInfo(char* buffer, size_t bufferSize) {
    const char* tzString = config[PARAM_SECONDARY_TZ].as<const char*>();

    if (!tzString) {
        buffer[0] = '\0';
        return false;
    }

    const char* separatorPos = strchr(tzString, '|');

    if (separatorPos) {
        size_t len = (size_t)(separatorPos - tzString);
        len = (len < bufferSize - 1) ? len : bufferSize - 1;
        strncpy(buffer, tzString, len);
        buffer[len] = '\0';
        Serial.printf("Timezone info: %s\n", buffer);
        return len;
    }

    return 0;
}

String AppClockOnly::getSecondaryTimeZone()
{
    char* tzString = strstr(config[PARAM_SECONDARY_TZ].as<const char*>(), "|");
    return tzString ? tzString + 1 : config[PARAM_SECONDARY_TZ].as<String>();
}


String AppClockOnly::getSecondaryTime()
{
    char buffer[32];
    const char* currentTZ = getenv("TZ");
    String previousTZ = currentTZ ? String(currentTZ) : "";
    String tzInfo;

    setenv("TZ", getSecondaryTimeZone().c_str(), 1);
    tzset();
    time_t now;
    struct tm timeinfo;
    now = hal.now;
    localtime_r(&now, &timeinfo);
    int offset = extractTimezoneInfo(buffer, sizeof(buffer));

    snprintf(buffer + offset, sizeof(buffer) - offset, "%02d:%02d",
             timeinfo.tm_hour,
             timeinfo.tm_min);

    Serial.printf("Secondary time: %s\n", buffer);
    // explicitly restore original TZ
    if (previousTZ.length()) {
        setenv("TZ", previousTZ.c_str(), 1);
        tzset();
    } else {
        unsetenv("TZ");
        tzset();
    }

    return String(buffer);
}
