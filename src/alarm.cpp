#include <alarm.h>
#include <SPIFFS.h>

Alarm alarms;

void Alarm::load()
{
    // 从SPIFFS加载
    if (SPIFFS.exists("/alarms.bin") == false)
    {
        clearAll();
        save();
        return;
    }
    File f = SPIFFS.open("/alarms.bin");
    f.readBytes((char *)alarm_table, sizeof(alarm_table));
    f.close();
}

void Alarm::save()
{
    File f = SPIFFS.open("/alarms.bin", "w");
    f.write((uint8_t *)alarm_table, sizeof(alarm_table));
    f.close();
}

int Alarm::getNext(uint16_t week, uint16_t now)
{
    uint16_t bit_week = BIT(week);
    int next = -1;
    uint16_t next_time = 0;
    for (int i = 0; i < 5; ++i)
    {
        if (alarm_table[i].enable & bit_week)
        {
            if (alarm_table[i].time > now)
            {
                if (next == -1)
                {
                    next = i;
                    next_time = alarm_table[i].time;
                }
                else
                {
                    if (alarm_table[i].time < next_time)
                    {
                        next = i;
                        next_time = alarm_table[i].time;
                    }
                }
            }
        }
    }
    return next;
}
