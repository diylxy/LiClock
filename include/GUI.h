#pragma once

namespace GUI
{
    extern int last_buffer_idx;
    void autoIndentDraw(const char *str, int max_x, int start_x = 2);
    void drawWindowsWithTitle(int16_t x = 0, int16_t y = 0, int16_t w = 296, int16_t h = 128, const char *title = NULL);
    bool msgbox_yn(const char *title, const char *msg, const char *yes = NULL, const char *no = NULL);
    int menu(const char *title, const char *options[]);
} // namespace GUI
