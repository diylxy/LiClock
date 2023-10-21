#ifndef __WEATHER_H__
#define __WEATHER_H__
#include <Arduino.h>
extern const char *weather_codes[];
extern const char *weather_icons[];
typedef struct
{
    char date[9];
    uint16_t weathernum;    // 天气类型序号
    int16_t temperature;    // 温度，定点1位小数
    uint16_t windspeed;     // 风速，定点1位小数，单位km/h
    uint16_t winddirection; // 风向，整数，指面向北方顺时针旋转的角度
    uint16_t rain;          // 降水量，单位毫米/24小时，整数
    uint16_t humidity;      // 湿度，百分数
    float pressure;

} weatherInfo24H;
typedef struct
{
    uint16_t weathernum;
    int16_t max;
    int16_t min;
} weatherInfoDaily;
typedef struct
{
    uint16_t weathernum;
    int16_t temperature;
    uint16_t humidity;
    float pressure;
} weatherInfoRealtime;
class Weather
{
private:
    uint8_t updateDate;

public:
    void begin();
    void save();
    int8_t refresh();
    uint16_t codeToNum(const char *code);
    weatherInfo24H *getWeather(uint8_t month, uint8_t date, uint8_t hour);
    void getRealIndex(int hourNow, int *index_ptr)
    {
        unsigned tmp = lastupdate % 86400 / 3600;
        if(hourNow >= tmp)
            *index_ptr += (hourNow - tmp);
    }

    /*
    数据说明
    温度：扩大10倍，单位摄氏度，有符号整数
    湿度：扩大100倍，单位%，无符号整数
    风速：扩大10倍，单位km/h，无符号整数
    降水量：扩大100倍，单位mm/h，无符号整数
    风向：无符号整数，指面向北方顺时针旋转的角度
    天气现象：无符号整数，对应天气现象表
    */
    weatherInfo24H hour24[20];
    uint16_t rain[120]; // 降水量，定点2位小数，单位mm/h
    weatherInfoDaily five_days[4];
    weatherInfoRealtime realtime;
    char desc1[100]; // 自然语言天气描述·综合
    char desc2[100]; // 自然语言天气描述·分钟级
    bool hasAlert = false;
    char alert[300];      // 天气预警
    char alertTitle[100]; // 天气预警标题
    uint32_t alertPubTime = 0;
    time_t lastupdate = 0;
};
extern Weather weather;

#endif
/*
小时级别：https://api.caiyunapp.com/v2.6/TAkhjf8d1nlSlspN/107.9752,34.2303/hourly?hourlysteps=72&unit=metric:v2
包含：
自然语言描述：
    json位置：$.result.hourly.description
降水：
    降水量 mm/h	 天气现象
    <0.08       无雨／雪
    0.08~3.44	小雨／雪
    3.44~11.33	中雨／雪
    11.33~51.30	大雨／雪
    >=51.30  	暴雨／雪
    图表中统一最大5mm/h，超过后自动查找最高点缩放图表
    json位置：$.result.hourly.precipitation.value
气温：（暂时用不到）
    json位置：$.result.hourly.temperature.value
天气现象：
    json位置：$.result.hourly.skycon.value

天气现象表：
主要天气现象的优先级：降雪 > 降雨 > 雾 > 沙尘 > 浮尘 > 雾霾 > 大风 > 阴 > 多云 > 晴
天气现象	代码	备注
晴（白天）	CLEAR_DAY	cloudrate < 0.2
晴（夜间）	CLEAR_NIGHT	cloudrate < 0.2
多云（白天）	PARTLY_CLOUDY_DAY	0.8 >= cloudrate > 0.2
多云（夜间）	PARTLY_CLOUDY_NIGHT	0.8 >= cloudrate > 0.2
阴	CLOUDY	cloudrate > 0.8
轻度雾霾	LIGHT_HAZE	PM2.5 100~150
中度雾霾	MODERATE_HAZE	PM2.5 150~200
重度雾霾	HEAVY_HAZE	PM2.5 > 200
小雨	LIGHT_RAIN	见 降水强度
中雨	MODERATE_RAIN	见 降水强度
大雨	HEAVY_RAIN	见 降水强度
暴雨	STORM_RAIN	见 降水强度
雾	FOG	能见度低，湿度高，风速低，温度低
小雪	LIGHT_SNOW	见 降水强度
中雪	MODERATE_SNOW	见 降水强度
大雪	HEAVY_SNOW	见 降水强度
暴雪	STORM_SNOW	见 降水强度
浮尘	DUST	AQI > 150, PM10 > 150，湿度 < 30%，风速 < 6 m/s
沙尘	SAND	AQI > 150, PM10> 150，湿度 < 30%，风速 > 6 m/s
大风	WIND

分钟级：https://api.caiyunapp.com/v2.6/TAkhjf8d1nlSlspN/101.6656,39.2072/minutely?unit=metric:v2
包含：
分钟级降水
    json位置：$.result.minutely.precipitation_2h
    数组，每分钟一个数据
分钟级降水描述：
    json位置：$.result.minutely.description
*/