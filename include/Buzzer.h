#pragma once
#include <Arduino.h>
typedef struct
{
    int16_t freq;
    int16_t duration;
} buzzer_desc;

class Buzzer
{
public:
    void init();
    bool hasNote();
    void waitForSleep();
    void append(int freq, int duration);
    void playFile(const char *path);
    void forceStop();
};
extern Buzzer buzzer;