#include "AppManager.h"

class AppLogViewer : public AppBase
{
private:
    /* data */
public:
    AppLogViewer()
    {
        name = "logviewer";
        title = "查看日志";
        description = "查看日志";
        image = NULL;
    }
    void setup();
};
static AppLogViewer app;
static void applogviewer_exit()
{
    appManager.noDeepSleep = false;
}
void AppLogViewer::setup()
{
    exit = applogviewer_exit;
    display.clearScreen();
    u8g2Fonts.setCursor(0, 15);
    u8g2Fonts.setFontMode(1);
    //u8g2Fonts.print(LogStr);
    display.display(false);
    appManager.noDeepSleep = true;
}
