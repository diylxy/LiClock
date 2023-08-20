#pragma once

typedef struct
{
    const uint8_t *icon;        //12*12图标,XBM格式
    const char *title;          //标题
} menu_item;

namespace GUI
{
    extern int last_buffer_idx;
    bool waitLongPress(int btn); // 检查长按，如果是长按则返回true
    void autoIndentDraw(const char *str, int max_x, int start_x = 2);
    void drawWindowsWithTitle(int16_t x = 0, int16_t y = 0, int16_t w = 296, int16_t h = 128, const char *title = NULL);
    void msgbox(const char *title, const char *msg);
    bool msgbox_yn(const char *title, const char *msg, const char *yes = NULL, const char *no = NULL);
    int msgbox_number(const char *title, uint16_t digits, int pre_value); // 注意digits，1表示一位，2表示两位，程序中减一
    int menu(const char *title, const menu_item options[], int16_t ico_w = 8, int16_t ico_h = 8);
    //fileManager.cpp
    const char *fileDialog(const char *title);
} // namespace GUI
