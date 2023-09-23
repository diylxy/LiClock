#pragma once
#include <Arduino.h>

enum alarm_enable_enum : uint16_t
{
    ALARM_ENABLE_SUNDAY = 0b00000001,
    ALARM_ENABLE_MONDAY = 0b00000010,
    ALARM_ENABLE_TUESDAY = 0b00000100,
    ALARM_ENABLE_WEDNESDAY = 0b00001000,
    ALARM_ENABLE_THURSDAY = 0b00010000,
    ALARM_ENABLE_FRIDAY = 0b00100000,
    ALARM_ENABLE_SATDAY = 0b01000000,
    ALARM_ENABLE_ONCE = 0b10000000,
    ALARM_DISABLE = 0b00000000,
};
typedef struct
{
    uint16_t time;
    enum alarm_enable_enum enable;
} alarm_t;

class Alarm
{
private:
public:
    alarm_t alarm_table[5];
    void load();
    void save();
    void clearAll()
    {
        memset(alarm_table, 0, sizeof(alarm_table));
    }
    int8_t getNext(uint16_t week, uint16_t now); // 返回距离现在最近的闹钟序号，如果没有则返回-1
    void alarm();                             // 闹钟界面
    void check();
    int getNextWakeupMinute();
    String getEnable(alarm_t *alarm);
};
extern Alarm alarms;