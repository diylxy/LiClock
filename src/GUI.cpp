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
            if (u8g2Fonts.getCursorX() >= max_x || *str == '\n')
            {
                u8g2Fonts.setCursor(start_x, u8g2Fonts.getCursorY() + 13);
            }
            if (*str != '\n')
            {
                u8g2Fonts.print(*str);
            }
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
    void drawWindowsWithTitle(const char *title, int16_t x, int16_t y, int16_t w, int16_t h)
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
    ////////////////////////////////////标准对话框

    void msgbox(const char *title, const char *msg)
    {
        // 160*100窗口，圆角5
        constexpr int start_x = (296 - 160) / 2;
        constexpr int start_y = (128 - 96) / 2;
        int16_t w;
        bool result = false;
        hal.hookButton();
        push_buffer();
        drawWindowsWithTitle(title, start_x, start_y, 160, 96);
        // 内容
        if (msg)
        {
            u8g2Fonts.setCursor(start_x + 5, start_y + 28);
            autoIndentDraw(msg, start_x + 160 - 5, start_x + 5);
        }
        // 按钮
        display.drawRoundRect(start_x + 85, start_y + 96 - 20, 70, 15, 3, 0);
        w = u8g2Fonts.getUTF8Width("确定");
        u8g2Fonts.setCursor(start_x + 85 + (70 - w) / 2, start_y + 96 - 20 + 12);
        u8g2Fonts.print("确定");
        display.displayWindow(start_x, start_y, 160, 96);
        while (1)
        {
            if (digitalRead(PIN_BUTTONR) == LOW || digitalRead(PIN_BUTTONL) == LOW || digitalRead(PIN_BUTTONC) == LOW)
                break;
            delay(10);
        }
        pop_buffer();
        hal.unhookButton();
    }
    bool msgbox_yn(const char *title, const char *msg, const char *yes, const char *no)
    {
        // 160*100窗口，圆角5
        constexpr int start_x = (296 - 160) / 2;
        constexpr int start_y = (128 - 96) / 2;
        if (yes == NULL)
            yes = "确定 (右)";
        if (no == NULL)
            no = "取消 (左)";
        int16_t w;
        bool result = false;
        hal.hookButton();
        push_buffer();
        drawWindowsWithTitle(title, start_x, start_y, 160, 96);
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
        while (1)
        {
            if (digitalRead(PIN_BUTTONR) == LOW)
            {
                result = true;
                break;
            }
            if (digitalRead(PIN_BUTTONL) == LOW)
            {
                result = false;
                break;
            }
            delay(10);
        }
        pop_buffer();
        hal.unhookButton();
        return result;
    }
    int menu(const char *title, const menu_item options[], int16_t ico_w, int16_t ico_h)
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
        bool hasIcon = false;
        bool waitc = false;
        while (options[total].title != NULL)
        {
            // 统计一共多少，顺便检查是否有图标
            if (options[total].icon != NULL)
                hasIcon = true;
            ++total;
        }
        barHeight = number_of_items * 96 / total;
        hal.hookButton();
        push_buffer();
        while (1)
        {
            if (digitalRead(PIN_BUTTONL) == LOW)
            {
                delay(20);
                if (digitalRead(PIN_BUTTONL) == LOW)
                {
                    if (selected == 0)
                    {
                        selected = total;
                    }
                    --selected;
                    updated = true;
                }
            }

            if (digitalRead(PIN_BUTTONR) == LOW)
            {
                delay(20);
                if (digitalRead(PIN_BUTTONR) == LOW)
                {
                    ++selected;
                    if (selected == total)
                    {
                        selected = 0;
                    }
                    updated = true;
                }
            }

            if (digitalRead(PIN_BUTTONC) == LOW)
            {
                delay(20);
                if (digitalRead(PIN_BUTTONC) == LOW)
                {
                    if (waitLongPress(PIN_BUTTONC) == true)
                    {
                        selected = 0;
                        waitc = true;
                        updated = true;
                    }
                    else
                    {
                        break;
                    }
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
                drawWindowsWithTitle(title, start_x, start_y, 200, 111);
                // 项目
                int max_items = min(number_of_items, total);
                for (int i = 0; i < max_items; ++i)
                {
                    int item_y = start_y + 15 + item_height * i;
                    if (options[i + pageStart].icon != NULL && ico_h <= 14)
                    {
                        display.drawXBitmap(start_x + 5, item_y + (14 - ico_h) / 2, options[i + pageStart].icon, ico_w, ico_h, 0);
                    }
                    u8g2Fonts.drawUTF8(start_x + 5 + (hasIcon ? ico_w + 2 : 0), item_y + 13, options[i + pageStart].title);
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
            if (waitc == true)
            {
                waitc = false;
                while (digitalRead(PIN_BUTTONC) == LOW)
                    delay(10);
                delay(10);
            }
            delay(10);
        }
        pop_buffer();
        hal.unhookButton();
        return selected;
    }
    int msgbox_number(const char *title, uint16_t digits, int pre_value) // 注意digits，1表示一位，2表示两位，程序中减一
    {
        constexpr int window_w = 120;
        constexpr int window_h = 48;
        constexpr int start_x = (296 - window_w) / 2;
        constexpr int start_y = (128 - window_h) / 2;
        constexpr int input_x = start_x + 5;
        constexpr int input_y = start_y + 18;
        constexpr int input_w = window_w - 10;
        constexpr int input_h = window_h - 18 - 3;
        if (digits <= 0)
            return 0;
        --digits;
        if (digits > 8)
            digits = 8;
        hal.hookButton();
        push_buffer();
        int currentNumber = pre_value;
        int current_digit = 0; // 0：个位
        int current_digit_10pow = 1;
        bool changed = true;
        while (1)
        {
            if (digitalRead(PIN_BUTTONL) == LOW)
            {
                // 减
                if (waitLongPress(PIN_BUTTONL))
                {
                    if (current_digit == digits)
                    {
                        current_digit = 0;
                    }
                    else
                    {
                        current_digit++;
                    }
                }
                else
                {
                    currentNumber -= current_digit_10pow;
                }
                changed = true;
            }
            else if (digitalRead(PIN_BUTTONR) == LOW)
            {
                // 加
                if (waitLongPress(PIN_BUTTONR))
                {
                    if (current_digit == 0)
                    {
                        current_digit = digits;
                    }
                    else
                    {
                        --current_digit;
                    }
                }
                else
                {
                    currentNumber += current_digit_10pow;
                }
                changed = true;
            }
            else if (digitalRead(PIN_BUTTONC) == LOW)
            {
                if (waitLongPress(PIN_BUTTONC))
                {
                    currentNumber = pre_value;
                    changed = true;
                }
                else
                {
                    break;
                }
            }
            if (changed)
            {
                // 计算当前位置
                current_digit_10pow = 1;
                if (current_digit != 0)
                {
                    for (int i = 0; i < current_digit; ++i)
                    {
                        current_digit_10pow *= 10;
                    }
                }
                changed = false;
                display.fillRoundRect(start_x, start_y, window_w, window_h, 3, 1);
                GUI::drawWindowsWithTitle(title, start_x, start_y, window_w, window_h);
                display.drawRoundRect(input_x, input_y, input_w, input_h, 3, 0);
                display.setFont(&FreeSans9pt7b);
                display.setTextColor(0);
                display.setCursor(input_x + 4, input_y + (input_h - 12) / 2 + 12);
                int currentNumber1 = currentNumber;
                if (currentNumber1 < 0)
                {
                    display.print('-');
                    currentNumber1 = -currentNumber1;
                }
                uint8_t tmp[9];
                for (int i = 0; i <= digits; ++i)
                {
                    tmp[i] = currentNumber1 % 10;
                    currentNumber1 /= 10;
                }
                for (int i = digits; i >= 0; --i)
                {
                    if (i == current_digit)
                    {
                        display.drawFastHLine(display.getCursorX(), display.getCursorY() + 2, 10, 0);
                    }
                    display.print(tmp[i], DEC);
                }
                display.displayWindow(start_x, start_y, window_w, window_h);
            }
            delay(10);
        }
        pop_buffer();
        hal.unhookButton();
        display.display(); // 全局刷新一次
        return currentNumber;
    }
    int msgbox_time(const char *title, int pre_value)
    {
        constexpr int window_w = 120;
        constexpr int window_h = 48;
        constexpr int start_x = (296 - window_w) / 2;
        constexpr int start_y = (128 - window_h) / 2;
        constexpr int input_x = start_x + 5;
        constexpr int input_y = start_y + 18;
        constexpr int input_w = window_w - 10;
        constexpr int input_h = window_h - 18 - 3;
        char timeBuffer[4];
        int16_t digit_add[4] = {1, 10, 60, 600};
        hal.hookButton();
        push_buffer();
        uint8_t current_digit = 0; // 0：个位
        int current_value = pre_value;
        bool changed = true;
        while (1)
        {
            if (digitalRead(PIN_BUTTONL) == LOW)
            {
                // 减
                if (waitLongPress(PIN_BUTTONL))
                {
                    if (current_digit == 3)
                    {
                        current_digit = 0;
                    }
                    else
                    {
                        current_digit++;
                    }
                }
                else
                {
                    current_value -= digit_add[current_digit];
                    if(current_value <= 0)
                    {
                        current_value = 0;
                    }
                }
                changed = true;
            }
            else if (digitalRead(PIN_BUTTONR) == LOW)
            {
                // 加
                if (waitLongPress(PIN_BUTTONR))
                {
                    if (current_digit == 0)
                    {
                        current_digit = 3;
                    }
                    else
                    {
                        --current_digit;
                    }
                }
                else
                {
                    current_value += digit_add[current_digit];
                    if(current_value >= 24*60)
                    {
                        current_value = 24*60 - 1;
                    }
                }
                changed = true;
            }
            else if (digitalRead(PIN_BUTTONC) == LOW)
            {
                if (waitLongPress(PIN_BUTTONC))
                {
                    current_value = pre_value;
                    changed = true;
                }
                else
                {
                    break;
                }
            }
            if (changed)
            {
                timeBuffer[3] = (current_value / 60) / 10;
                timeBuffer[2] = (current_value / 60) % 10;
                timeBuffer[1] = (current_value % 60) / 10;
                timeBuffer[0] = (current_value % 60) % 10;
                // 计算当前位置
                changed = false;
                display.fillRoundRect(start_x, start_y, window_w, window_h, 3, 1);
                GUI::drawWindowsWithTitle(title, start_x, start_y, window_w, window_h);
                display.drawRoundRect(input_x, input_y, input_w, input_h, 3, 0);
                display.setFont(&FreeSans9pt7b);
                display.setTextColor(0);
                display.setCursor(input_x + 4, input_y + (input_h - 12) / 2 + 12);
                for (int i = 3; i >= 0; --i)
                {
                    if (i == current_digit)
                    {
                        display.drawFastHLine(display.getCursorX(), display.getCursorY() + 2, 10, 0);
                    }
                    display.print(timeBuffer[i], DEC);
                }
                display.displayWindow(start_x, start_y, window_w, window_h);
            }
            delay(10);
        }
        pop_buffer();
        hal.unhookButton();
        display.display(); // 全局刷新一次
        return current_value;
    }
    void drawLBM(int16_t x, int16_t y, const char *filename, uint16_t color)
    {
        FILE *fp = fopen(getRealPath(filename), "rb");
        if (!fp)
        {
            Serial.printf("File %s not found!\n", filename);
            return;
        }
        uint16_t w, h;
        fread(&w, 2, 1, fp);
        fread(&h, 2, 1, fp);
        size_t imgsize;
        uint16_t tmp = w / 8;
        if (w % 8 != 0)
            tmp++;
        imgsize = tmp * h;
        uint8_t *img = (uint8_t *)malloc(imgsize);
        if (!img)
        {
            Serial.printf("malloc failed!\n");
            fclose(fp);
            return;
        }
        fread(img, 1, imgsize, fp);
        fclose(fp);
        display.drawXBitmap(x, y, img, w, h, color);
        free(img);
    }
} // namespace GUI