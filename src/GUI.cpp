#include <A_Config.h>

namespace GUI
{
    int last_buffer_idx = 0;
    bool waitLongPress(int btn) // 检查长按，如果是长按则返回true
    {
        for (int16_t i = 0; i < 60; ++i)
        {
            if (digitalRead(btn) == 1)
                return false;
            delay(10);
        }
        return true;
    }
    // 自动换行
    void autoIndentDraw(const char *str, int max_x, int start_x)
    {
        while (*str)
        {
            if (u8g2Fonts.getCursorX() >= max_x)
            {
                u8g2Fonts.setCursor(start_x, u8g2Fonts.getCursorY() + 13);
            }
            u8g2Fonts.print(*str);
            str++;
        }
    }
    inline void push_buffer()
    {
        last_buffer_idx = display.current_buffer_idx;
        display.swapBuffer(2);
        display.copyBuffer(2, last_buffer_idx);
    }
    inline void pop_buffer()
    {
        display.swapBuffer(last_buffer_idx);
    }
    void drawWindowsWithTitle(int16_t x, int16_t y, int16_t w, int16_t h, const char *title)
    {
        int16_t wchar;
        display.fillRoundRect(x, y, w, h, 3, 1); // 清空区域
        display.drawRoundRect(x, y, w, h, 3, 0);
        // 标题栏
        display.drawFastHLine(x, y + 14, w, 0);
        if (title)
        {
            u8g2Fonts.setBackgroundColor(1);
            u8g2Fonts.setForegroundColor(0);
            u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
            wchar = u8g2Fonts.getUTF8Width(title);
            u8g2Fonts.setCursor(x + (w - wchar) / 2, y + 12);
            u8g2Fonts.print(title);
        }
    }
    bool msgbox_yn(const char *title, const char *msg, const char *yes, const char *no)
    {
        // 160*100窗口，圆角5
        constexpr int start_x = (296 - 160) / 2;
        constexpr int start_y = (128 - 96) / 2;
        if(yes == NULL)
            yes = "确定 (A)";
        if(no == NULL)
            no = "取消 (B)";
        int16_t w;
        bool result = false;
        push_buffer();
        drawWindowsWithTitle(start_x, start_y, 160, 96, title);
        // 内容
        u8g2Fonts.setCursor(start_x + 5, start_y + 28);
        autoIndentDraw(msg, start_x + 160 - 5, start_x + 5);
        // 按钮
        display.drawRoundRect(start_x + 5, start_y + 96 - 20, 70, 15, 3, 0);
        display.drawRoundRect(start_x + 85, start_y + 96 - 20, 70, 15, 3, 0);
        w = u8g2Fonts.getUTF8Width(no);
        u8g2Fonts.setCursor(start_x + 5 + (70 - w) / 2, start_y + 96 - 20 + 12);
        u8g2Fonts.print(no);
        w = u8g2Fonts.getUTF8Width(yes);
        u8g2Fonts.setCursor(start_x + 85 + (70 - w) / 2, start_y + 96 - 20 + 12);
        u8g2Fonts.print(yes);
        display.displayWindow(start_x, start_y, 160, 96);
        hal.hookButton = true;
        delay(40);
        while (1)
        {
            if (digitalRead(PIN_BUTTON) == LOW)
            {
                result = true;
                break;
            }
            if (digitalRead(PIN_BUTTONB) == LOW)
            {
                result = false;
                break;
            }
            delay(10);
        }
        hal.hookButton = false;
        pop_buffer();
        return result;
    }
    int menu(const char *title, const char *options[])
    {
        constexpr int start_x = (296 - 200) / 2;
        constexpr int start_y = (128 - 111) / 2; // 200*96
        constexpr int number_of_items = 6;
        constexpr int item_height = (96) / number_of_items; // 16
        constexpr int item_width = 200 - 5 - 5 - 5;         // 右侧滚动条
        int pageStart = 0;                                  // 屏幕顶部第一个
        int selected = 0;
        int total = 0;
        int barHeight;
        int barPos = 0;
        bool updated = true;
        while (options[++total] != NULL)
            ; // 统计一共多少
        barHeight = number_of_items * 96 / total;
        hal.hookButton = true;
        push_buffer();
        while (digitalRead(PIN_BUTTON) == 0 || digitalRead(PIN_BUTTONB) == 0)
        {
            delay(10);
        }
        delay(40);
        while (1)
        {
            if (digitalRead(PIN_BUTTONB) == 0)
            {
                delay(20);
                if (digitalRead(PIN_BUTTONB) == 0)
                {
                    if (waitLongPress(PIN_BUTTONB) == true)
                    {
                        selected = 0;
                        break;
                    }
                    if (selected == 0)
                    {
                        selected = total;
                    }
                    --selected;
                    updated = true;
                }
            }

            if (digitalRead(PIN_BUTTON) == 0)
            {
                delay(20);
                if (digitalRead(PIN_BUTTON) == 0)
                {
                    if (waitLongPress(PIN_BUTTON) == true)
                    {
                        break;
                    }
                    ++selected;
                    if (selected == total)
                    {
                        selected = 0;
                    }
                    updated = true;
                }
            }

            if (updated == true)
            {
                updated = false;
                // 判断是否出界
                if (selected < pageStart)
                {
                    pageStart = selected;
                }
                else if (selected >= pageStart + number_of_items)
                {
                    pageStart = selected - number_of_items + 1;
                }
                // 下面渲染菜单
                drawWindowsWithTitle(start_x, start_y, 200, 111, title);
                // 项目
                int max_items = min(number_of_items, total);
                for (int i = 0; i < max_items; ++i)
                {
                    int item_y = start_y + 15 + item_height * i;
                    u8g2Fonts.drawUTF8(start_x + 5, item_y + 13, options[i + pageStart]);
                    if (selected == i + pageStart)
                    {
                        display.drawRoundRect(start_x + 3, item_y, 195 - 6, 15, 3, 0);
                    }
                }
                // 滚动条
                // 以Selected为基准
                if (total > number_of_items)
                {
                    barPos = selected * (96 - barHeight) / total;
                    display.fillRoundRect(start_x + 195 + 1, start_y + 15 + barPos, 3, barHeight, 2, 0);
                }
                display.displayWindow(start_x, start_y, 200, 111);
            }
        }
        pop_buffer();
        hal.hookButton = false;
        return selected;
    }
    int msgbox_number(const char *title, uint16_t digits, int pre_value)
    {
        return 0;           //TODO：需要三个按钮
        int currentNumber = pre_value;
        int current_digit = 0;      //0：个位
        hal.hookButton = true;
        while (digitalRead(PIN_BUTTON) == 0 || digitalRead(PIN_BUTTONB) == 0)
        {
            delay(10);
        }
        push_buffer();
        delay(20);
        while(1)
        {
            if (digitalRead(PIN_BUTTONB) == 0)
            {
                //减
            }
            if (digitalRead(PIN_BUTTON) == 0)
            {
                //加
            }
        }
        hal.hookButton = false;
        pop_buffer();
    }
} // namespace GUI