#pragma once
#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SGP30.h>
#include <Wire.h>
#include <SD.h>
#include <DS3231.h>
#define PERIPHERALS_SD_BIT 1
#define PERIPHERALS_AHT20_BIT 2
#define PERIPHERALS_BMP280_BIT 4
#define PERIPHERALS_SGP30_BIT 8
#define PERIPHERALS_DS3231_BIT 16

class Peripherals
{
private:
    /* data */
    bool ahtInited = false;
    bool bmpInited = false;
    bool sgpInited = false;
public:
    uint16_t peripherals_current = 0; // 当前设备安装的外设
    uint16_t peripherals_load = 0;    // 当前已经加载的外设
    void check();                              // 重新检测外设情况
    void init();                               // 系统加载时从闪存加载外设信息
    void initSGP();                             // 初始化SGP30
    uint16_t checkAvailable(uint16_t bitmask); // 检测请求的外设是否存在，如果存在则返回0,否则返回缺失的外设
    bool load(uint16_t bitmask);           // 按照bitmask加载外设
    void sleep();                              // 进入休眠模式，禁用所有外设
    void wakeup();                             // 尝试从休眠模式恢复之前的状态
    bool isSDLoaded()
    {
        return (peripherals_load & PERIPHERALS_SD_BIT) == PERIPHERALS_SD_BIT;
    }
    void load_append(uint16_t bitmask);
    // 下面是传感器实例
    Adafruit_AHTX0 aht;
    Adafruit_BMP280 bmp;
    Adafruit_SGP30 sgp;
    DS3231 rtc;
    SemaphoreHandle_t i2cMutex = NULL;
};
extern Peripherals peripherals;
