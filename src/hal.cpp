#include "hal.h"
#include <SPIFFS.h>

void task_hal_update(void *)
{
    while (1)
    {
        if (hal.hookButton)
        {
            while (digitalRead(PIN_BUTTON) == 0 || digitalRead(PIN_BUTTONB) == 0)
            {
                hal.btna.tick();
                hal.btnb.tick();
                delay(10);
            }
            hal.btna.tick();
            hal.btnb.tick();
            while (hal.hookButton)
            {
                while (hal.SleepUpdateMutex)
                    delay(5);
                hal.update();
                delay(10);
            }
            while (digitalRead(PIN_BUTTON) == 0 || digitalRead(PIN_BUTTONB) == 0)
            {
                delay(10);
            }
        }
        while (hal.SleepUpdateMutex)
            delay(5);
        hal.SleepUpdateMutex = true;
        hal.btna.tick();
        hal.btnb.tick();
        hal.SleepUpdateMutex = false;
        delay(10);
        while (hal.SleepUpdateMutex)
            delay(5);
        hal.SleepUpdateMutex = true;
        hal.btna.tick();
        hal.btnb.tick();
        hal.update();
        hal.SleepUpdateMutex = false;
        delay(10);
    }
}
static void setSleepGPIO()
{
    rtc_gpio_init((gpio_num_t)PIN_BUTTON);
    rtc_gpio_pullup_en((gpio_num_t)PIN_BUTTON);
}
void HAL::saveConfig()
{
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
        Serial.println("Failed to open config file for writing");
        return;
    }
    serializeJson(config, configFile);
    configFile.close();
}
void HAL::loadConfig()
{
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return;
    }
    deserializeJson(config, configFile);
    configFile.close();
}

void HAL::getTime()
{
    int64_t tmp;
    time(&now);
    if (delta != 0 && lastsync < now)
    {
        // 下面修正时钟频率偏移
        tmp = now - lastsync;
        tmp *= delta;
        tmp /= every;
        now -= tmp;
    }
    localtime_r(&now, &timeinfo);
}

void WiFiConfigSmartConfig()
{
#include "img_esptouch.h"
    display.fillScreen(GxEPD_WHITE);
    display.drawXBitmap(0, 0, esptouch_bits, 296, 128, GxEPD_BLACK);
    display.display();
    WiFi.beginSmartConfig();
    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        ++count;
        if (count >= 240) // 120秒超时
        {
            Serial.println("SmartConfig超时");
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.setCursor(70, 80);
            u8g2Fonts.print("SmartConfig超时");
            display.display();
            delay(100);
            hal.powerOff(false);
            ESP.restart();
        }
    }
    Serial.println("WiFi connected");
    config[PARAM_SSID] = WiFi.SSID();
    config[PARAM_PASS] = WiFi.psk();
    hal.saveConfig();
}

#include <DNSServer.h>
void HAL::WiFiConfigManual()
{
    DNSServer dnsServer;
#include "img_manual.h"
    String passwd = String((esp_random() % 1000000000L) + 10000000L); // 生成随机密码
    String str = "WIFI:T:WPA2;S:WeatherClock;P:" + passwd + ";;";
    WiFi.softAP("WeatherClock", passwd.c_str());
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
    dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
    beginWebServer();
    display.fillScreen(GxEPD_WHITE);
    display.drawXBitmap(0, 0, wifi_manual_bits, 296, 128, GxEPD_BLACK);
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(7)];
    qrcode_initText(&qrcode, qrcodeData, 6, 0, str.c_str());
    Serial.println(qrcode.size);
    for (uint8_t y = 0; y < qrcode.size; y++)
    {
        // Each horizontal module
        for (uint8_t x = 0; x < qrcode.size; x++)
        {
            display.fillRect(2 * x + 20, 2 * y + 20, 2, 2, qrcode_getModule(&qrcode, x, y) ? GxEPD_BLACK : GxEPD_WHITE);
        }
    }
    display.setFont(&FreeSans9pt7b);
    display.setCursor(192, 124);
    display.print(passwd);
    display.display();
    uint32_t last_millis = millis();
    while (1)
    {
        dnsServer.processNextRequest();
        updateWebServer();
        delay(5);
        if (millis() - last_millis > 600000) // 10分钟超时
        {
            Serial.println("手动配置超时");
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.setCursor(70, 80);
            u8g2Fonts.print("手动配置超时");
            display.display();
            delay(100);
            hal.powerOff(false);
            ESP.restart();
        }
    }
}
void HAL::ReqWiFiConfig()
{
    display.fillScreen(GxEPD_WHITE);
    u8g2Fonts.setCursor(0, 20);
    u8g2Fonts.print("无法连接到WiFi");
    u8g2Fonts.setCursor(0, 40);
    u8g2Fonts.print("按键A:SmartConfig");
    u8g2Fonts.setCursor(0, 60);
    u8g2Fonts.print("按键B:网页配置");
    display.display();
    uint32_t last_millis = millis();
    while (1)
    {
        if (digitalRead(PIN_BUTTONB) == 0)
        {
            WiFiConfigManual();
        }
        if (digitalRead(PIN_BUTTON) == 0)
        {
            WiFiConfigSmartConfig();
        }
        delay(5);
        if (millis() - last_millis > 60000) // 1分钟超时
        {
            Serial.println("WiFi配置方式选择超时");
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.setCursor(70, 80);
            u8g2Fonts.print("WiFi配置方式选择超时");
            display.display();
            delay(100);
            hal.powerOff(false);
            ESP.restart();
        }
    }
}
bool HAL::init()
{
    bool timeerr = false;
    bool initial = true;
    setenv("TZ", "CST-8", 1);
    tzset();
    // 读取时钟偏移
    pref.begin("clock");
    if (pref.getUInt("lastsync") == 0)
    {
        pref.putUInt("lastsync", 1);  // 上次同步时间的准确时间
        pref.putInt("every", 100);    // 两次校准间隔多久
        pref.putInt("delta", 0);      // 这两次校准之间时钟偏差秒数，时钟时间-准确时间
        pref.putInt("upint", 2 * 60); // NTP同步间隔
    }
    lastsync = pref.getUInt("lastsync", 1); // 上次同步时间的准确时间
    every = pref.getInt("every", 100);      // 两次校准间隔多久
    delta = pref.getInt("delta", 0);        // 这两次校准之间时钟偏差秒数，时钟时间-准确时间
    upint = pref.getInt("upint", 2 * 60);   // NTP同步间隔
    getTime();
    // 系统“自检”
    if ((timeinfo.tm_year < (2016 - 1900)))
    {
        timeerr = true;              // 需要同步时间
        pref.putUInt("lastsync", 1); // 清除上次同步时间，但不清除时钟偏移信息
        lastsync = 1;
    }
    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_UNDEFINED)
        initial = false;
    // 下面进行初始化
    rtc_gpio_deinit((gpio_num_t)PIN_BUTTON);
    esp_task_wdt_init(portMAX_DELAY, false);
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_BUTTONB, INPUT_PULLUP);
    pinMode(PIN_USB, INPUT_PULLUP);
    xTaskCreate(task_hal_update, "hal_update", 2048, NULL, 10, NULL);
    Serial.begin(115200);
    WiFi.mode(WIFI_OFF);
    display.init(0, initial);
    display.setRotation(3);
    display.setTextColor(GxEPD_BLACK);
    u8g2Fonts.setFontMode(1);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312b);
    u8g2Fonts.begin(display);
    if (digitalRead(PIN_BUTTONB) == 0 && (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_UNDEFINED))
    {
        // 关机，防止后面程序出问题
        powerOff(true);
        ESP.restart();
    }
    if (SPIFFS.begin(false) == false)
    {
        display.fillScreen(GxEPD_WHITE);
        u8g2Fonts.setCursor(70, 80);
        u8g2Fonts.print("等待SPIFFS格式化");
        display.display();
        SPIFFS.format();
        if (SPIFFS.begin(false) == false)
        {
            Serial.println("SPIFFS格式化失败");
            display.fillScreen(GxEPD_WHITE);
            u8g2Fonts.setCursor(70, 80);
            u8g2Fonts.print("SPIFFS格式化失败");
            display.display(true);
            delay(100);
            powerOff(false);
            ESP.restart();
        }
        Serial.println("正在写入默认配置");
        File f = SPIFFS.open("/config.json", "w");
        f.print(DEFAULT_CONFIG);
        f.close();
    }
    loadConfig();
    weather.begin();
    if (initial == false && timeerr == false)
    {
        return false;
    }
    return true;
}

void HAL::goSleep(uint32_t sec)
{
    hal.getTime();
    long nextSleep = 0;
    if (sec != 0)
        nextSleep = sec;
    else
    {
        nextSleep = 1;
    }
    Serial.printf("next wakeup:%ld s\n", nextSleep);
    Serial.println("sleep");
    nextSleep = nextSleep * 1000000UL;
    setSleepGPIO();
    esp_sleep_enable_timer_wakeup(nextSleep);
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON, 0);
    display.hibernate();
    delay(10);
    if (noDeepSleep)
    {
        esp_light_sleep_start();
        display.init(0, false);
    }
    else
    {
        esp_deep_sleep_start();
    }
}

void HAL::autoConnectWiFi()
{
    if (WiFi.isConnected())
    {
        return;
    }
    // 下面连接WiFi
    if (config[PARAM_SSID] == "")
    {
        ReqWiFiConfig();
    }
    else
    {
        WiFi.setHostname("weatherclock");
        WiFi.mode(WIFI_STA);
        WiFi.begin(config[PARAM_SSID].as<const char *>(), config[PARAM_PASS].as<const char *>());
    }
    if (!WiFi.isConnected())
    {
        if (WiFi.waitForConnectResult(20000) != WL_CONNECTED)
        {
            hal.ReqWiFiConfig();
        }
    }
    sntp_stop();
}

void HAL::powerOff(bool displayMessage)
{
    if (displayMessage)
    {
        display.setFullWindow();
        display.fillScreen(GxEPD_WHITE);
        u8g2Fonts.setCursor(120, 70);
        u8g2Fonts.print("已关机");
        display.display();
        display.hibernate();
        delay(70);
    }
    else
    {
        display.hibernate();
        delay(70);
    }
    force_full_update = true;
    WiFi.disconnect(true);
    setSleepGPIO();
    esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON, 0);
    delay(10);
    if (noDeepSleep)
    {
        esp_light_sleep_start();
        display.init(0, false);
    }
    else
    {
        esp_deep_sleep_start();
    }
}

void HAL::update(void)
{
    getTime();
    long adc;
    adc = analogRead(PIN_ADC);
    adc = adc * 6600 / 4096;
    VCC = adc;
    if (adc > 4400)
    {
        USBPluggedIn = true;
    }
    else
    {
        USBPluggedIn = false;
    }
    if (digitalRead(PIN_USB) == 0)
    {
        isCharging = true;
    }
    else
    {
        isCharging = false;
    }
}

HAL hal;