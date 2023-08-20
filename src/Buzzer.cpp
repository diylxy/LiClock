#include <A_Config.h>
#include <Buzzer.h>
Buzzer buzzer;
QueueHandle_t queue_buzzer = NULL;
bool _hasNote = false;
char _path[128];
bool useFile = false;
bool _forcestop = false;
void task_buzzer(void *)
{
    buzzer_desc current;
    while (1)
    {
        if (xQueueReceive(queue_buzzer, &current, portMAX_DELAY) == pdTRUE)
        {
            if (current.freq == -1 && current.duration == -1)
            {
                // 播放文件
                if (_path[0])
                {
                    FILE *fp = fopen(_path, "rb");
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
                        fread(&current, sizeof(buzzer_desc), 1, fp);
                        ledcWriteTone(0, current.freq);
                        delay(current.duration);
                        ledcWriteTone(0, 0);
                        if (feof(fp))
                            break;
                        if (_forcestop)
                        {
                            _forcestop = false;
                            while (xQueueReceive(queue_buzzer, &current, 0) == pdTRUE)
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
                while (xQueueReceive(queue_buzzer, &current, 0) == pdTRUE)
                    ;
                continue;
            }
            // 播放
            _hasNote = true;
            ledcWriteTone(0, current.freq);
            delay(current.duration);
            ledcWriteTone(0, 0);
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
    ledcAttachPin(PIN_BUZZER, 0);
}

bool Buzzer::hasNote()
{
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