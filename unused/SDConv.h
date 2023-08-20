#pragma once

#include <A_Config.h>

class SDConv
{
    private:
        uint16_t _end = 0;
    public:
        const char *OEM2UTF8(const char *str);
        const char *UTF82OEM(const char *str);
        char _buffer[512];
};
extern SDConv sdconv;