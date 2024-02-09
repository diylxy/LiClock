#include <A_Config.h>
#include <Buzzer.h>
Buzzer buzzer;
QueueHandle_t queue_buzzer = NULL;
bool _hasNote = false;
char _path[128];
bool useFile = false;
bool _forcestop = false;
static buzzer_desc current_buzz;
static uint32_t current_start = 0;
static void _mybuzz()
{
    int32_t now_duty = 511;
    if (current_buzz.freq == 0)
    {
        delay(current_buzz.duration);
    }
    else
    {
        ledcWriteTone(0, current_buzz.freq);
        current_start = millis();
        while (millis() - current_start < current_buzz.duration)
        {
            ledcWrite(0, now_duty);
            if (now_duty > 200)
                now_duty -= 20;
            else if (now_duty > 100)
                now_duty -= 15;
            else if (now_duty > 50)
                now_duty -= 10;
            else if (now_duty > 10)
                now_duty -= 7;
            else if (now_duty > 0)
                now_duty -= 4;
            else
                now_duty = 0;
            delay(20);
        }
    }
    ledcWriteTone(0, 0);
}
void task_buzzer(void *)
{
    while (1)
    {
        if (xQueueReceive(queue_buzzer, &current_buzz, portMAX_DELAY) == pdTRUE)
        {
            if (current_buzz.freq == -1 && current_buzz.duration == -1)
            {
                // 播放文件
                if (_path[0])
                {
                    FILE *fp = fopen(getRealPath(_path), "rb");
                    if (fp == NULL)
                    {
                        Serial.printf("Buzzer: Failed to open file %s\n", _path);
                        useFile = false;
                        _path[0] = 0;
                        continue;
                    }
                    while (1)
                    {
                        if (feof(fp))
                            break;
                        fread(&current_buzz, sizeof(buzzer_desc), 1, fp);
                        _mybuzz();
                        if (feof(fp))
                            break;
                        if (_forcestop)
                        {
                            _forcestop = false;
                            while (xQueueReceive(queue_buzzer, &current_buzz, 0) == pdTRUE)
                                ;
                            break;
                        }
                    }
                    fclose(fp);
                }
                useFile = false;
                _path[0] = 0;
                continue;
            }
            if (_forcestop)
            {
                _forcestop = false;
                ledcWriteTone(0, 0);
                while (xQueueReceive(queue_buzzer, &current_buzz, 0) == pdTRUE)
                    ;
                continue;
            }
            // 播放
            _hasNote = true;
            _mybuzz();
            _hasNote = false;
        }
    }
}

void Buzzer::waitForSleep()
{
    while (hasNote() == true)
        delay(10);
}
void Buzzer::init()
{
    queue_buzzer = xQueueCreate(100, sizeof(buzzer_desc)); // 我知道ESP32的tone是用队列实现的，但是觉得不方便就写了个壳
    xTaskCreate(task_buzzer, "task_buzzer", 4096, NULL, 10, NULL);
    ledcSetup(0, 48000, 10);
    ledcAttachPin(PIN_BUZZER, 0);
}

bool Buzzer::hasNote()
{
    if (!queue_buzzer)
        return false;
    return (_hasNote || uxQueueMessagesWaiting(queue_buzzer) != 0 || useFile == true);
}
void Buzzer::append(int freq, int duration)
{
    buzzer_desc add;
    add.freq = freq;
    add.duration = duration;
    xQueueSendToBack(queue_buzzer, &add, portMAX_DELAY);
}
void Buzzer::playFile(const char *path)
{
    strcpy(_path, path);
    useFile = true;
    buzzer_desc addFile;
    addFile.freq = -1;
    addFile.duration = -1;
    xQueueSendToBack(queue_buzzer, &addFile, portMAX_DELAY);
}

void Buzzer::forceStop()
{
    _forcestop = true;
}