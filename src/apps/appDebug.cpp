#include "AppManager.h"
static const uint8_t debug_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0xfe, 0xff, 0x00,
   0x80, 0xff, 0xff, 0x01, 0xc0, 0x07, 0xe0, 0x03, 0xe0, 0x01, 0x80, 0x07,
   0xf0, 0x01, 0x00, 0x0f, 0xf8, 0x03, 0x00, 0x1e, 0xf8, 0xc7, 0xe3, 0x1c,
   0x9c, 0xff, 0x7f, 0x38, 0x1c, 0xff, 0x3f, 0x38, 0x0e, 0xfe, 0x1f, 0x70,
   0x0e, 0xfc, 0x3f, 0x70, 0x0e, 0xf8, 0xff, 0x71, 0x0e, 0xf0, 0xff, 0x71,
   0x0e, 0xe0, 0x3f, 0x70, 0x0e, 0xc0, 0x3f, 0x70, 0x8e, 0x87, 0xff, 0x71,
   0x8e, 0x0f, 0xff, 0x71, 0x0e, 0x1c, 0x3e, 0x70, 0x0e, 0x38, 0x7c, 0x70,
   0x1c, 0x7c, 0xf8, 0x38, 0x1c, 0xfe, 0xf0, 0x39, 0x38, 0xc7, 0xe1, 0x1f,
   0x78, 0x00, 0xc0, 0x1f, 0xf0, 0x00, 0x80, 0x0f, 0xe0, 0x01, 0x80, 0x07,
   0xc0, 0x07, 0xe0, 0x03, 0x80, 0xff, 0xff, 0x01, 0x00, 0xfe, 0xff, 0x00,
   0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00 };


class AppDebug : public AppBase
{
private:
    /* data */
public:
    AppDebug()
    {
        name = "debug";
        title = "Debug";
        description = "Debug专用";
        image = debug_bits;
    }
    void setup();
};
static AppDebug app;

void AppDebug::setup()
{
    const char *menuItems[] = {"测试选项1", "测试选项2", "测试选项3","测试选项4", "测试选项5", "测试选项6","测试选项7", "测试选项8", "测试选项9", NULL};
    int sel = GUI::menu("测试", menuItems);
    display.clearScreen();
    u8g2Fonts.setCursor(10, 10);
    u8g2Fonts.print(sel);
    display.display(false);
}
