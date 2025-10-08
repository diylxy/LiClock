#include <A_Config.h>
// 之前写的程序，一次获取，内存不足可尝试分批
Weather weather;
#define WEATHER_TYPE_COUNT 20
const char *weather_codes[WEATHER_TYPE_COUNT] =
    {
        "CLEAR_DAY",
        "CLEAR_NIGHT",
        "PARTLY_CLOUDY_DAY",
        "PARTLY_CLOUDY_NIGHT",
        "CLOUDY",
        "LIGHT_HAZE",
        "MODERATE_HAZE",
        "HEAVY_HAZE",
        "LIGHT_RAIN",
        "MODERATE_RAIN",
        "HEAVY_RAIN",
        "STORM_RAIN",
        "FOG",
        "LIGHT_SNOW",
        "MODERATE_SNOW",
        "HEAVY_SNOW",
        "STORM_SNOW",
        "DUST",
        "SAND",
        "WIND",
};
void Weather::begin()
{
    File file = LittleFS.open("/weather.bin", "r");
    if (!file)
    {
        Serial.println("无法打开天气文件，或天气不存在");
        return;
    }
    file.readBytes((char *)&hour24, sizeof(hour24));
    file.readBytes((char *)&rain, sizeof(rain));
    file.readBytes((char *)&five_days, sizeof(five_days));
    file.readBytes((char *)&realtime, sizeof(realtime));
    file.readBytes((char *)&desc1, sizeof(desc1));
    file.readBytes((char *)&desc2, sizeof(desc2));
    file.readBytes((char *)&hasAlert, sizeof(hasAlert));
    file.readBytes((char *)alert, sizeof(alert));
    file.readBytes((char *)alertTitle, sizeof(alertTitle));
    file.readBytes((char *)&alertPubTime, sizeof(alertPubTime));
    file.readBytes((char *)&lastupdate, sizeof(lastupdate));

    file.close();
}
void Weather::save()
{
    File file = LittleFS.open("/weather.bin", "w");
    if (!file)
    {
        Serial.println("无法写入天气文件");
        return;
    }
    file.write((uint8_t *)&hour24, sizeof(hour24));
    file.write((uint8_t *)&rain, sizeof(rain));
    file.write((uint8_t *)&five_days, sizeof(five_days));
    file.write((uint8_t *)&realtime, sizeof(realtime));
    file.write((uint8_t *)&desc1, sizeof(desc1));
    file.write((uint8_t *)&desc2, sizeof(desc2));
    file.write((uint8_t *)&hasAlert, sizeof(hasAlert));
    file.write((uint8_t *)alert, sizeof(alert));
    file.write((uint8_t *)alertTitle, sizeof(alertTitle));
    file.write((uint8_t *)&alertPubTime, sizeof(alertPubTime));
    file.write((uint8_t *)&lastupdate, sizeof(lastupdate));
    file.close();
}

int8_t Weather::refresh()
{
    HTTPClient http;
    http.begin(String("http://api.caiyunapp.com/v2.5/96Ly7wgKGq6FhllM/") + config[PARAM_GPS].as<String>() + String("/weather.jsonp?hourlysteps=20&unit=metric%3Av2&dailysteps=4&alert=true")); // HTTP
    http.addHeader("Accept", "*/*");
    http.addHeader("User-Agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.131 Safari/537.36");

    hal.autoConnectWiFi();
    Serial.println("开始更新天气");
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        JsonDocument doc;
        auto s = http.getStream();
        deserializeJson(doc, s);
        if (doc["status"] != "ok")
        { // API失效
            http.end();
            doc.clear();
            Serial.println("天气API已失效");
            return -3;
        }
        if (doc["result"]["alert"]["status"] == "ok")
        {
            if (doc["result"]["alert"]["content"].size() != 0)
            {
                hasAlert = true;
                strcpy(alert, doc["result"]["alert"]["content"][0]["description"].as<const char *>());
                strcpy(alertTitle, doc["result"]["alert"]["content"][0]["title"].as<const char *>());
                alertPubTime = doc["result"]["alert"]["content"][0]["pubtimestamp"].as<uint32_t>();
            }
        }
        else
        {
            hasAlert = false;
        }
        strcpy(desc1, doc["result"]["hourly"]["description"].as<String>().c_str());
        strcpy(desc2, doc["result"]["minutely"]["description"].as<String>().c_str());
        for (uint8_t i = 0; i < 20; ++i)
        {
            // 下面获取当前时间
            String timestr;
            timestr = doc["result"]["hourly"]["temperature"][i]["datetime"].as<String>();
            timestr = timestr.substring(5, 13);
            strcpy(hour24[i].date, timestr.c_str());
            // 天气类型、气温、风速
            String s = doc["result"]["hourly"]["skycon"][i]["value"].as<String>();
            hour24[i].weathernum = codeToNum(s.c_str());
            hour24[i].temperature = int16_t(doc["result"]["hourly"]["temperature"][i]["value"].as<float>() * 10);
            hour24[i].winddirection = uint16_t(doc["result"]["hourly"]["wind"][i]["direction"].as<float>());
            hour24[i].windspeed = uint16_t(doc["result"]["hourly"]["wind"][i]["speed"].as<float>() * 10);
            hour24[i].rain = uint16_t(doc["result"]["hourly"]["precipitation"][i]["value"].as<float>() * 100);
            // 气压
            hour24[i].pressure = doc["result"]["hourly"]["pressure"][i]["value"];
        }
        for (uint8_t i = 0; i < 120; ++i)
        {
            // 降水
            rain[i] = doc["result"]["minutely"]["precipitation_2h"][i].as<float>() * 100;
        }
        for (uint8_t i = 0; i < 4; ++i)
        {
            Serial.println(doc["result"]["daily"]["temperature"][i]["date"].as<String>());
            five_days[i].max = int16_t(doc["result"]["daily"]["temperature"][i]["max"].as<float>() * 10);
            five_days[i].min = int16_t(doc["result"]["daily"]["temperature"][i]["min"].as<float>() * 10);
            five_days[i].weathernum = codeToNum(doc["result"]["daily"]["skycon"][i]["value"].as<String>().c_str());
        }
        // 实时天气
        realtime.weathernum = codeToNum(doc["result"]["realtime"]["skycon"].as<String>().c_str());
        realtime.temperature = int16_t(doc["result"]["realtime"]["temperature"].as<float>() * 10);
        realtime.humidity = uint16_t(doc["result"]["realtime"]["humidity"].as<float>() * 100);
        realtime.pressure = doc["result"]["realtime"]["pressure"];
        doc.clear();
        lastupdate = hal.now;
        save();
        Serial.println("天气更新成功");
    }
    else
    {
        http.end();
        Serial.println("HTTP错误");
        return -2;
    }
    http.end();
    return 0;
}

weatherInfo24H *Weather::getWeather(uint8_t month, uint8_t date, uint8_t hour)
{
    char strdate[9];
    sprintf(strdate, "%02d-%02dT%02d", month, date, hour);
    for (uint8_t i = 0; i < 48; ++i)
    {
        if (strcmp(strdate, hour24[i].date) == 0)
        {
            return &hour24[i];
        }
    }
    return NULL;
}

uint16_t Weather::codeToNum(const char *code)
{
    for (uint8_t i = 0; i < WEATHER_TYPE_COUNT; ++i)
    {
        if (strcmp(weather_codes[i], code) == 0)
        {
            return i;
        }
    }
    return 0;
}