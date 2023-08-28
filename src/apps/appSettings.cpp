#include "AppManager.h"
static const uint8_t settings_bits[] = {
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0xff, 0x03,
    0x00, 0x80, 0xc7, 0x03, 0x00, 0xc0, 0xe3, 0x01, 0x00, 0xc0, 0xf1, 0x00,
    0x00, 0xc0, 0x78, 0x70, 0x00, 0xe0, 0x38, 0x78, 0x00, 0xe0, 0x38, 0xfc,
    0x00, 0x60, 0x30, 0xfe, 0x00, 0xe0, 0xf0, 0xef, 0x00, 0xe0, 0xf0, 0xe7,
    0x00, 0xe0, 0x80, 0x63, 0x00, 0x70, 0x00, 0x70, 0x00, 0x38, 0x00, 0x38,
    0x00, 0x1c, 0x00, 0x3c, 0x00, 0x0e, 0xb8, 0x1f, 0x00, 0x07, 0xfc, 0x0f,
    0x80, 0x03, 0xfe, 0x01, 0xc0, 0x01, 0x07, 0x00, 0xe0, 0x80, 0x03, 0x00,
    0x70, 0xc0, 0x01, 0x00, 0x38, 0xe0, 0x00, 0x00, 0x1c, 0x70, 0x00, 0x00,
    0x0e, 0x38, 0x00, 0x00, 0x07, 0x1c, 0x00, 0x00, 0x27, 0x0e, 0x00, 0x00,
    0x07, 0x07, 0x00, 0x00, 0x86, 0x03, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00,
    0xfc, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00};

static const menu_item settings_menu_main[] =
    {
        {NULL, "退出"},
        {NULL, "时间设置"},
        {NULL, "闹钟设置"},
        {NULL, "网络设置"},
        {NULL, "外设设置"},
        {NULL, "其它设置"},
        {NULL, "关于"},
        {NULL, NULL},
};

static const menu_item settings_menu_time[] =
    {
        {NULL, "返回上一级"},
        {NULL, "手动触发NTP"},
        {NULL, "时间同步间隔设置"},
        {NULL, "RTC线性偏移修正"},
        {NULL, "离线模式"},
        {NULL, NULL},
};
static const menu_item settings_menu_network[] =
    {
        {NULL, "返回上一级"},
        {NULL, "ESPTouch配网"},
        {NULL, "启动HTTP服务器"},
        {NULL, "ESPNow设备扫描"},
        {NULL, "蓝牙扫描"},
        {NULL, "退出Bilibili账号"},
        {NULL, NULL},
};

static const menu_item settings_menu_peripheral[] =
    {
        {NULL, "返回上一级"},
        {NULL, "重新扫描外设"},
        {NULL, "AHT20"},
        {NULL, "BMP280"},
        {NULL, "SGP30"},
        {NULL, "DS3231"},
        {NULL, "SD卡"},
        {NULL, NULL},
};

static const menu_item settings_menu_other[] =
    {
        {NULL, "返回上一级"},
        {NULL, "屏幕方向"},
        {NULL, "天气更新间隔"},
        {NULL, "主屏幕应用选择"},
        {NULL, "已安装应用管理"},
        {NULL, "系统信息"},
        {NULL, "恢复出厂设置"},
        {NULL, NULL},
};

class AppSettings : public AppBase
{
private:
    /* data */
    String toApp = "";
    bool hasToApp = false;

public:
    AppSettings()
    {
        name = "settings";
        title = "设置";
        description = "简单的设置";
        image = settings_bits;
    }
    void setup();
    void menu_time();
    void menu_alarm();
    void menu_network();
    void menu_peripheral();
    void menu_other();
};
static AppSettings app;

void AppSettings::setup()
{
    display.clearScreen();
    GUI::drawWindowsWithTitle("设置", 0, 0, 296, 128);
    u8g2Fonts.drawUTF8(120, 75, "请稍等...");
    // 下面是设置菜单
    int res = 0;
    bool end = false;
    while (end == false && hasToApp == false)
    {
        display.display(false); // 每次进入设置全局刷新一次
        res = GUI::menu("设置", settings_menu_main);
        switch (res)
        {
        case 0:
            end = true;
            break;
        case 1:
            // 时间设置
            menu_time();
            break;
        case 2:
            // 闹钟设置
            menu_alarm();
            break;
        case 3:
            // 网络设置
            menu_network();
            break;
        case 4:
            // 外设设置
            menu_peripheral();
            break;
        case 5:
            // 其它设置
            menu_other();
            break;
        default:
            break;
        }
    }
    if (hasToApp == true)
    {
        hasToApp = false;
        if (toApp != "")
        {
            appManager.gotoApp(toApp.c_str());
        }
        return;
    }
    appManager.goBack();
}

// 时间设置
void AppSettings::menu_time()
{
    int res = 0;
    bool end = false;
    while (end == false && hasToApp == false)
    {
        res = GUI::menu("时间设置", settings_menu_time);
        switch (res)
        {
        case 0:
            end = true;
            break;
        case 1:
            // 手动触发NTP
            if (GUI::msgbox_yn("手动触发NTP", "将连接WiFi并同步时间") == true)
            {
                // 同步时间
                hal.autoConnectWiFi();
                NTPSync();
                GUI::msgbox("手动触发NTP", "同步完成");
            }
            break;
        case 2:
            // 时间同步间隔设置
            {
                const menu_item menu[] = {
                    {NULL, "取消"},
                    {NULL, "禁用时间同步"},
                    {NULL, "2小时"},
                    {NULL, "4小时"},
                    {NULL, "6小时"},
                    {NULL, "12小时"},
                    {NULL, "24小时"},
                    {NULL, "36小时"},
                    {NULL, "48小时"},
                    {NULL, NULL},
                };
                res = GUI::menu("时间同步间隔设置", menu);
                if (res > 0)
                {
                    // 设置时间同步间隔
                    hal.pref.putUChar(SETTINGS_PARAM_NTP_INTERVAL, res - 1);
                    GUI::msgbox("时间同步间隔设置", "设置完成");
                }
            }
            break;
        case 3:
            // RTC线性偏移修正
            toApp = "rtcoffset";
            hasToApp = true;
            end = true;
            break;
        case 4:
            // 离线模式
            if (GUI::msgbox_yn("仅时钟模式", "是否启用仅时钟模式？", "仅时钟", "天气时钟"))
            {
                // 启用
                config[PARAM_CLOCKONLY] = "1";
                hal.saveConfig();
            }
            else
            {
                config[PARAM_CLOCKONLY] = "0";
                hal.saveConfig();
            }
        default:
            break;
        }
    }
}

// 闹钟设置
void AppSettings::menu_alarm()
{
    int res = 0;
    bool end = false;
    menu_item settings_menu_alarm[] = {
        {NULL, "返回上一级"},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
    };
    char alarm_buf[5][20];
    char alarm_buf_week[25];
    char bit_week[7] = {0};
    while (end == false && hasToApp == false)
    {
        // 读取闹钟设置
        for (int i = 0; i < 5; ++i)
        {
            if (alarms.alarm_table[i].time == 0)
            {
                sprintf(alarm_buf[i], "%d：未设置", i + 1);
            }
            else
            {
                sprintf(alarm_buf[i], "%d：%02d:%02d", i + 1, alarms.alarm_table[i].time / 60, alarms.alarm_table[i].time % 60);
            }
            settings_menu_alarm[i + 1].title = alarm_buf[i];
        }
        res = GUI::menu("闹钟设置", settings_menu_alarm);
        if (res == 0)
            break;
    }
}

// 网络设置
void AppSettings::menu_network()
{
    int res = 0;
    bool end = false;
    while (end == false && hasToApp == false)
    {
        res = GUI::menu("网络设置", settings_menu_network);
        switch (res)
        {
        case 0:
            end = true;
            break;
        case 1:
            // ESPTouch配网
            hal.WiFiConfigSmartConfig();
            break;
        case 2:
            // 启动HTTP服务器
            toApp = "webserver";
            hasToApp = true;
            end = true;
            break;
        case 3:
            // ESPNow设备扫描
            break;
        case 4:
            // 蓝牙扫描
            break;
        case 5:
            // 退出Bilibili账号
            break;
        default:
            break;
        }
    }
}

// 外设设置
void AppSettings::menu_peripheral()
{
    int res = 0;
    bool end = false;
    while (end == false && hasToApp == false)
    {
        res = GUI::menu("外设设置", settings_menu_peripheral);
        switch (res)
        {
        case 0:
            end = true;
            break;
        case 1:
            // 重新扫描外设
            peripherals.check();
            break;
        case 2:
            // AHT20
            if (peripherals.checkAvailable(PERIPHERALS_AHT20_BIT) == 0)
            {
                toApp = "demoaht20";
                hasToApp = true;
                end = true;
            }
            break;
        case 3:
            // BMP280
            if (peripherals.checkAvailable(PERIPHERALS_BMP280_BIT) == 0)
            {
                toApp = "demobmp280";
                hasToApp = true;
                end = true;
            }
            break;
        case 4:
            // SGP30
            break;
        case 5:
            // DS3231
            break;
        case 6:
            // SD卡
            break;
        default:
            break;
        }
    }
}

#include <nvs_flash.h>
// 其它设置
void AppSettings::menu_other()
{
    int res = 0;
    bool end = false;
    while (end == false && hasToApp == false)
    {
        res = GUI::menu("其它设置", settings_menu_other);
        switch (res)
        {
        case 0:
            end = true;
            break;
        case 1:
            // 屏幕方向
            if (GUI::msgbox_yn("屏幕方向选择", "正常意为开关在屏幕左上，否则为右下", "正常（右）", "反转（左）"))
            {
                hal.pref.putUChar(SETTINGS_PARAM_SCREEN_ORIENTATION, 3);
            }
            else
            {
                hal.pref.putUChar(SETTINGS_PARAM_SCREEN_ORIENTATION, 1);
            }
            display.display();
            GUI::msgbox("提示", "按键控制方式请修改GPIO宏定义"); // 为了节省内存并加快速度
            break;
        case 2:
            // 天气更新间隔
            {
                int res = GUI::msgbox_number("请输入天气更新间隔", 2, atoi(config[PARAM_FULLUPDATE].as<const char *>()));
                if (res <= 5 || res > 40)
                {
                    res = 20;
                }
                char tmp[4];
                sprintf(tmp, "%d", res);
                config[PARAM_FULLUPDATE] = tmp;
                hal.saveConfig();
                break;
            }
        case 3:
            // 主屏幕应用选择
            break;
        case 4:
            // 已安装应用管理
            break;
        case 6:
            // 系统信息
            break;
        case 7:
            // 恢复出厂设置
            {
                if (GUI::msgbox_yn("此操作不可撤销", "是否恢复出厂设置？"))
                {
                    if (GUI::msgbox_yn("这是最后一次提示", "将格式化nvs和SPIFFS存储区", "取消（右）", "确认（左）") == false)
                    {
                        display.clearScreen();
                        u8g2Fonts.drawUTF8(30, 40, "正在格式化NVS存储");
                        display.display();
                        nvs_flash_erase();
                        u8g2Fonts.drawUTF8(30, 40, "正在格式化SPIFFS存储");
                        display.display(true);
                        SPIFFS.end();
                        SPIFFS.format();
                        u8g2Fonts.drawUTF8(30, 40, "完成，正在重启");
                        display.display(true);
                        ESP.restart();
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}