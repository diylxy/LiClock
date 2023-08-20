#pragma once
#include <Arduino.h>

enum alarm_enable_enum : uint16_t
{
    ALARM_ENABLE_MONDAY = 0b00000001,

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
    int getNext(uint16_t week, uint16_t now);   //返回距离现在最近的闹钟序号，如果没有则返回-1
};
extern Alarm alarms;