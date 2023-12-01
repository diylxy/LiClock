#include "AppManager.h"
static const uint8_t ebook_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x03,
    0x00, 0x00, 0x00, 0x06, 0x00, 0xfc, 0xff, 0x04, 0x00, 0x00, 0x80, 0x05,
    0x00, 0xff, 0x3f, 0x05, 0x80, 0x00, 0x40, 0x05, 0x80, 0x00, 0x40, 0x05,
    0x80, 0xf0, 0x4f, 0x05, 0x80, 0x00, 0x40, 0x05, 0x80, 0xfe, 0x4f, 0x05,
    0x80, 0x00, 0x40, 0x05, 0x80, 0x3e, 0x40, 0x05, 0x80, 0x00, 0x40, 0x05,
    0x80, 0x00, 0x40, 0x05, 0x80, 0xf0, 0x4f, 0x05, 0x80, 0x00, 0x40, 0x05,
    0x80, 0xfe, 0x4f, 0x05, 0x80, 0x00, 0x40, 0x05, 0x80, 0x3e, 0x40, 0x01,
    0x80, 0x00, 0x40, 0x01, 0x80, 0x00, 0x40, 0x00, 0x80, 0x01, 0x60, 0x00,
    0x00, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
class AppEBook : public AppBase
{
private:
    /* data */
public:
    AppEBook()
    {
        name = "ebook";
        title = "电子书";
        description = "简易电子书";
        image = ebook_bits;
        peripherals_requested = PERIPHERALS_SD_BIT;
        wakeupIO[0] = PIN_BUTTONL;
        wakeupIO[1] = PIN_BUTTONR;
        noDefaultEvent = true;
    }
    void setup();
    //////////////////////////
    bool indexFile();
    bool openFile(const char *filename = NULL);
    bool gotoPage(uint32_t page);
    void drawCurrentPage();
    int getTotalPages();
    void openMenu();
    FILE *indexFileHandle = NULL;
    FILE *currentFileHandle = NULL;
    size_t currentFileOffset = 0;
    char currentFilename[256];
    bool __eof = false;
};
RTC_DATA_ATTR uint32_t currentPage = 0; // 0:第一页
static AppEBook app;
static void appebook_exit()
{
    if (app.currentFileHandle != NULL)
    {
        fclose(app.currentFileHandle);
        app.currentFileHandle = NULL;
    }
    if (app.indexFileHandle != NULL)
    {
        fclose(app.indexFileHandle);
        app.indexFileHandle = NULL;
    }
    hal.pref.putInt(SETTINGS_PARAM_LAST_EBOOK_PAGE, currentPage);
    Serial.printf("退出电子书，当前页：%d\n", currentPage);
}
static void appebook_deepsleep()
{
    if (app.currentFileHandle != NULL)
    {
        fclose(app.currentFileHandle);
        app.currentFileHandle = NULL;
    }
    if (app.indexFileHandle != NULL)
    {
        fclose(app.indexFileHandle);
        app.indexFileHandle = NULL;
    }
}
// 左键：上一页
// 右键：下一页
// 长按右键：打开菜单

void AppEBook::setup()
{
    bool page_changed = false;
    app.exit = appebook_exit;
    app.deepsleep = appebook_deepsleep;
    app.currentFilename[0] = 0;
    display.clearScreen();
    size_t s = hal.pref.getBytes(SETTINGS_PARAM_LAST_EBOOK, app.currentFilename, 256);
    if (appManager.parameter == "")
    {
        currentPage = hal.pref.getInt(SETTINGS_PARAM_LAST_EBOOK_PAGE, 0);
        if (s == 0)
        {
            openFile();
        }
        else
        {
            Serial.printf("电子书：上次打开的文件：%s，上次打开的页：%d\n", app.currentFilename, currentPage);
            if (openFile(app.currentFilename) == false)
            {
                if (openFile() == false)
                {
                    GUI::msgbox("打开文件失败", currentFilename);
                    hal.pref.remove(SETTINGS_PARAM_LAST_EBOOK);
                    hal.pref.remove(SETTINGS_PARAM_LAST_EBOOK_PAGE);
                    appManager.goBack();
                }
            }
        }
        page_changed = true;
    }
    else
    {
        Serial.println("从DeepSleep唤醒");
        if (s == 0)
            appManager.goBack();
        if (app.openFile(app.currentFilename) == false)
            appManager.goBack();
    }
    gotoPage(currentPage);
    if (digitalRead(PIN_BUTTONL) == 0)
    {
        Serial.println("左键按下");
        if (currentPage == 0)
        {
            GUI::msgbox("提示", "已经是第一页了");
            display.display(true);
        }
        else if (gotoPage(currentPage - 1) == false)
        {
            GUI::msgbox("提示", "翻页发生错误");
            display.display(true);
        }
        else
        {
            page_changed = true;
        }
    }
    else if (digitalRead(PIN_BUTTONR) == 0)
    {
        Serial.println("右键按下");
        if (gotoPage(currentPage + 1) == false)
        {
            if (GUI::waitLongPress(PIN_BUTTONR))
            {
                Serial.println("长按右键");
                // 打开菜单
                openMenu();
                display.display(true);
                while (digitalRead(PIN_BUTTONL) == 0 || digitalRead(PIN_BUTTONR) == 0)
                {
                    delay(10);
                }
                return;
            }
            else
            {
                GUI::msgbox("提示", "已经是最后一页了");
                Serial.println("已经是最后一页了");
                display.display(true);
            }
        }
        else
        {
            page_changed = true;
        }
    }
    if (page_changed == true)
    {
        page_changed = false;
        drawCurrentPage();
    }
    if (GUI::waitLongPress(PIN_BUTTONR))
    {
        Serial.println("长按右键");
        // 打开菜单
        openMenu();
        display.display(true);
    }
    while (digitalRead(PIN_BUTTONL) == 0 || digitalRead(PIN_BUTTONR) == 0)
    {
        delay(10);
    }
}

// 索引格式
// 每4字节代表一页在某个文件中的起始位置
// 文件全部采用UTF 8编码
// 字符宽度：默认英文7,中文14

bool AppEBook::indexFile()
{
    if (currentFileHandle == NULL)
    {
        GUI::msgbox("索引错误", "请先打开文件");
        return false;
    }
    fseek(currentFileHandle, 0, SEEK_SET);
    uint32_t page = 0;   // 页码
    uint32_t offset = 0; // 主文件偏移量
    int16_t x = 0;       // 当前页中字符的x坐标
    int16_t y = 0;       // 当前页中字符的y坐标
    int c;
    String indexname = String(currentFilename) + ".i";
    indexFileHandle = fopen(indexname.c_str(), "wb");
    if (indexFileHandle == NULL)
    {
        Serial.println("打开索引文件失败");
        GUI::msgbox("打开索引文件失败", indexname.c_str());
        return false;
    }
    while (true)
    {
        c = fgetc(currentFileHandle);
        if (c == EOF)
        {
            break;
        }
        while(c == '\n')continue;
        offset++;
        int utf_bytes = 0;
        if (c & 0x80)
        {
            if (c & 0x40)
            {
                if (c & 0x20)
                {
                    if (c & 0x10)
                    {
                        if (c & 0x08)
                        {
                            if (c & 0x04)
                            {
                                utf_bytes = 5;
                            }
                            else
                            {
                                utf_bytes = 4;
                            }
                        }
                        else
                        {
                            utf_bytes = 3;
                        }
                    }
                    else
                    {
                        utf_bytes = 2;
                    }
                }
                else
                {
                    utf_bytes = 1;
                }
            }
            else
            {
                GUI::msgbox("索引错误", "非预期的UTF8编码");
                fclose(indexFileHandle);
                indexFileHandle = NULL;
                return false;
            }
        }
        if (utf_bytes != 0)
        {
            for (int i = 0; i < utf_bytes; i++)
            {
                fgetc(currentFileHandle);
                offset++;
            }
        }
        int add_pending;
        if (utf_bytes == 0)
        {
            add_pending = 6;
        }
        else
        {
            add_pending = 12;
        }
        if (c == '\n')
        {
            y += 14;
            x = 0;
            add_pending = 0;
        }
        else if (x + add_pending >= 296)
        {
            x = add_pending;
            y += 14;
        }
        else
        {
            x += add_pending;
        }
        if (y >= 128 - 14)
        {
            page++;
            x = add_pending;
            y = 0;
            uint32_t pageOffset = offset - utf_bytes - 1;
            fwrite(&pageOffset, 4, 1, indexFileHandle);
        }
    }
    fclose(indexFileHandle);
    indexFileHandle = NULL;
    char *tmp = (char *)malloc(256);
    sprintf(tmp, "共%d页, 最后一页偏移量：%d", page + 1, offset);
    GUI::msgbox("索引成功", tmp);
    free(tmp);
    indexFileHandle = fopen((String(currentFilename) + ".i").c_str(), "rb");
    if (indexFileHandle == NULL)
    {
        GUI::msgbox("索引失败", "无法再次打开索引文件");
        return false;
    }
    return true;
}

bool AppEBook::openFile(const char *filename)
{
    if (app.currentFileHandle != NULL)
    {
        fclose(app.currentFileHandle);
        app.currentFileHandle = NULL;
    }
    if (app.indexFileHandle != NULL)
    {
        fclose(app.indexFileHandle);
        app.indexFileHandle = NULL;
    }
    if (filename == NULL)
    {
        strcpy(currentFilename, GUI::fileDialog("请选择文件"));
    }
    else
    {
        strcpy(currentFilename, filename);
    }
    currentFileHandle = fopen(currentFilename, "rb");
    if (currentFileHandle == NULL)
    {
        return false;
    }
    indexFileHandle = fopen((String(currentFilename) + ".i").c_str(), "rb");
    if (indexFileHandle == NULL)
    {
        if (!indexFile())
        {
            return false;
        }
    }
    hal.pref.putBytes(SETTINGS_PARAM_LAST_EBOOK, app.currentFilename, strlen(app.currentFilename));
    return true;
}

bool AppEBook::gotoPage(uint32_t page)
{
    if (page == 0)
    {
        currentPage = 0;
        currentFileOffset = 0;
        fseek(currentFileHandle, 0, SEEK_SET);
        return true;
    }
    else if (indexFileHandle != NULL)
    {
        fseek(indexFileHandle, (page - 1) * 4, SEEK_SET);
        if (fread(&currentFileOffset, sizeof(currentFileOffset), 1, indexFileHandle) == 1)
        {
            currentPage = page;
            fseek(currentFileHandle, currentFileOffset, SEEK_SET);
            return true;
        }
        else
            __eof = true;
    }
    return false;
}
uint8_t RTC_DATA_ATTR partcount = 100;
void AppEBook::drawCurrentPage()
{
    int16_t x = 0, y = 0;
    // 窗口
    display.clearScreen();
    // 自动换行
    while (true)
    {
        int c = fgetc(currentFileHandle);
        if (c == EOF)
        {
            __eof = true;
            break;
        }
        while(c == '\n')continue;
        int utf_bytes = 0;
        if (c & 0x80)
        {
            if (c & 0x40)
            {
                if (c & 0x20)
                {
                    if (c & 0x10)
                    {
                        if (c & 0x08)
                        {
                            if (c & 0x04)
                            {
                                utf_bytes = 5;
                            }
                            else
                            {
                                utf_bytes = 4;
                            }
                        }
                        else
                        {
                            utf_bytes = 3;
                        }
                    }
                    else
                    {
                        utf_bytes = 2;
                    }
                }
                else
                {
                    utf_bytes = 1;
                }
            }
            else
            {
                GUI::msgbox("读文件错误", "非预期的UTF8编码");
                break;
            }
        }
        int add_pending;
        if (utf_bytes == 0)
        {
            add_pending = 6;
        }
        else
        {
            add_pending = 12;
        }
        if (c == '\n')
        {
            y += 14;
            x = 0;
            add_pending = 0;
        }
        else if (x + add_pending >= 296)
        {
            x = 0;
            y += 14;
        }
        if (y >= 128 - 14)
        {
            if (utf_bytes != 0)
            {
                for (int i = 0; i < utf_bytes; i++)
                {
                    fgetc(currentFileHandle);
                }
                break;
            }
        }
        if (c != '\n')
        {
            u8g2Fonts.setCursor(x, y + 13);
            x += add_pending;
            u8g2Fonts.write((uint8_t)c);
            if (utf_bytes != 0)
            {
                for (int i = 0; i < utf_bytes; i++)
                {
                    int c1 = fgetc(currentFileHandle);
                    if (c1 == EOF)
                    {
                        break;
                    }
                    u8g2Fonts.write((uint8_t)c1);
                }
            }
        }
    }
    if (partcount < 10)
    {
        display.display(true);
        ++partcount;
    }
    else
    {
        partcount = 0;
        display.display(false);
    }
}

int AppEBook::getTotalPages()
{
    struct stat fileStat;
    fstat(fileno(indexFileHandle), &fileStat);
    return fileStat.st_size / 4 + 1;
}
static int get_digits(int val)
{
    int digits = 0;
    while (val > 0)
    {
        val /= 10;
        digits++;
    }
    return digits;
}
void AppEBook::openMenu()
{
    char *title = (char *)malloc(128);
    int totalPages = getTotalPages();
    sprintf(title, "%d/%d %d%%", currentPage + 1, totalPages, (currentPage + 1) * 100 / totalPages);
    const menu_item items[] = {
        {NULL, "返回"},
        {NULL, "退出"},
        {NULL, "换文件.."},
        {NULL, "重建当前文件索引"},
        {NULL, "跳转到.."},
        {NULL, NULL},
    };
    int ret = GUI::menu(title, items);
    free(title);
    switch (ret)
    {
    case 0:
        break;
    case 1:
        appManager.goBack();
        return;
        break;
    case 2:
        if (openFile() == false)
        {
            GUI::msgbox("打开文件失败", currentFilename);
        }
        gotoPage(0);
        drawCurrentPage();
        break;
    case 3:
        display.clearScreen();
        display.setCursor(20, 20);
        display.setFont(&FreeSans9pt7b);
        display.print("Rebuilding index...");
        display.display(true);
        indexFile();
        gotoPage(0);
        drawCurrentPage();
        break;
    case 4:
    {
        int page = GUI::msgbox_number("跳转到..", get_digits(totalPages), currentPage + 1);
        if (page > 0)
        {
            if (gotoPage(page - 1) == false)
            {
                GUI::msgbox("跳转失败", "页码超出范围");
                gotoPage(currentPage);
            }
            drawCurrentPage();
        }
    }
    break;
    default:
        break;
    }
}
