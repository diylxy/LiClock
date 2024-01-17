#include "AppManager.h"

class AppBuzzer : public AppBase
{
private:
    /* data */
public:
    AppBuzzer()
    {
        name = "buzzer";
        title = "蜂鸣器";
        description = "蜂鸣器音乐播放器";
        image = NULL;
        peripherals_requested = PERIPHERALS_SD_BIT;
    }
    void setup();
};
static AppBuzzer app;

void AppBuzzer::setup()
{
    while (1)
    {
        const char *path = GUI::fileDialog("请选择要播放的文件", false, "buz");
        if (path == NULL)
        {
            appManager.goBack();
            return;
        }
        buzzer.playFile(path);
        display.display(false); // 全局刷新一次
        while (hal.btnl.isPressing() == false && hal.btnr.isPressing() == false && hal.btnc.isPressing() == false && buzzer.hasNote())
        {
            delay(100);
        }
        if (buzzer.hasNote())
        {
            buzzer.forceStop();
        }
        if (GUI::msgbox_yn("播放已停止", "是否退出播放", "退出", "换文件"))
        {
            break;
        }
    }
    appManager.goBack();
}
