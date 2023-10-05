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
        {NULL, "重新扫描外设"},
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
        noDefaultEvent = true;
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
            // 重新扫描外设
            peripherals.check();
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
        {NULL, "闹钟铃声"},
        {NULL, NULL},
    };
    const menu_item settings_menu_alarm_sub[] = {
        {NULL, "返回"},
        {NULL, "时间"},
        {NULL, "重复周期"},
        {NULL, NULL},
    };
    const menu_item settings_menu_alarm_time[] = {
        {NULL, "返回"},
        {NULL, "关闭"},
        {NULL, "单次"},
        {NULL, "周一到周五"},
        {NULL, "周六日"},
        {NULL, "周一"},
        {NULL, "周二"},
        {NULL, "周三"},
        {NULL, "周四"},
        {NULL, "周五"},
        {NULL, "周六"},
        {NULL, "周日"},
        {NULL, "手动输入"},
        {NULL, NULL},
    };
    char alarm_buf[5][30];
    char alarm_buf_week[25];
    char bit_week[7] = {0};
    while (end == false && hasToApp == false)
    {
        // 读取闹钟设置
        for (int i = 0; i < 5; ++i)
        {
            if (alarms.alarm_table[i].enable == 0)
            {
                sprintf(alarm_buf[i], "%d：%02d:%02d，关闭", i + 1, alarms.alarm_table[i].time / 60, alarms.alarm_table[i].time % 60, alarms.getEnable(alarms.alarm_table + i).c_str());
            }
            else
            {
                sprintf(alarm_buf[i], "%d：%02d:%02d,%s", i + 1, alarms.alarm_table[i].time / 60, alarms.alarm_table[i].time % 60, alarms.getEnable(alarms.alarm_table + i).c_str());
            }
            settings_menu_alarm[i + 1].title = alarm_buf[i];
        }
        res = GUI::menu("闹钟设置", settings_menu_alarm);
        if (res == 0)
            break;
        if (res == 6)
        {
            const char *str = GUI::fileDialog("请选择闹钟铃声文件", false, ".buz");
            if (str)
            {
                hal.pref.putString(SETTINGS_PARAM_ALARM_TONE, String(str));
            }
            else
            {
                if (GUI::msgbox_yn("你选择了返回", "是否使用默认铃声，或者保留之前的设置", "使用默认", "取消"))
                {
                    hal.pref.remove(SETTINGS_PARAM_ALARM_TONE);
                }
            }
        }
        int selected = res - 1;
        res = GUI::menu(alarm_buf[selected], settings_menu_alarm_sub);
        switch (res)
        {
        case 0:
            break;
        case 1:
        {
            alarms.alarm_table[selected].time = GUI::msgbox_time("请输入闹钟时间", alarms.alarm_table[selected].time);
            if (alarms.alarm_table[selected].enable == 0)
                alarms.alarm_table[selected].enable = ALARM_ENABLE_ONCE;
            break;
        }
        case 2:
        {
            int res;
            res = GUI::menu("请选择重复周期", settings_menu_alarm_time);
            enum alarm_enable_enum res_table[] = {
                ALARM_DISABLE,
                ALARM_ENABLE_ONCE,
                (enum alarm_enable_enum)0b00111110,
                (enum alarm_enable_enum)0b01000001,
                ALARM_ENABLE_MONDAY,
                ALARM_ENABLE_TUESDAY,
                ALARM_ENABLE_WEDNESDAY,
                ALARM_ENABLE_THURSDAY,
                ALARM_ENABLE_FRIDAY,
                ALARM_ENABLE_SATDAY,
                ALARM_ENABLE_SUNDAY,
            };
            switch (res)
            {
            case 0:
                break;
            case 12:
            {
                int time = GUI::msgbox_number("输入重复周期Bitmap", 3, alarms.alarm_table[selected].enable);
                alarms.alarm_table[selected].enable = (enum alarm_enable_enum)(time % 256);
            }
            break;
            default:
                alarms.alarm_table[selected].enable = (enum alarm_enable_enum)(res_table[(res - 1) % 11]);
                break;
            }
        }
        break;
        default:
            break;
        }
    }
    alarms.save();
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
            if (LittleFS.exists("/blCookies.txt"))
            {
                LittleFS.remove("/blCookies.txt");
                GUI::msgbox("完成", "Bilibili账户登录信息已删除");
                break;
            }
            else
            {
                GUI::msgbox("提示", "Bilibili Cookies不存在");
                break;
            }
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
            {
                AppBase *tmp = appManager.appSelector(true);
                if (tmp)
                {
                    Serial.println(tmp->name);
                    if (GUI::msgbox_yn("警告", "选择不兼容的App可能会导致无法开机，是否确认？") == true)
                    {
                        if(strcmp(tmp->name, "clock") == 0)
                        {
                            config[PARAM_CLOCKONLY] = "0";
                            hal.saveConfig();
                            hal.pref.putString(SETTINGS_PARAM_HOME_APP, "clock");
                        }
                        else if(strcmp(tmp->name, "clockonly") == 0)
                        {
                            config[PARAM_CLOCKONLY] = "0";
                            hal.saveConfig();
                            hal.pref.putString(SETTINGS_PARAM_HOME_APP, "clock");
                        }
                        else
                        {
                            hal.pref.putString(SETTINGS_PARAM_HOME_APP, tmp->name);
                        }
                        GUI::msgbox("设置成功", "重启或下次唤醒后生效");
                    }
                }
                break;
            }
        case 4:
            // 已安装应用管理
            toApp = "installer";
            hasToApp = true;
            end = true;
            return;
            break;
        case 5:
            // 恢复出厂设置
            {
                if (GUI::msgbox_yn("此操作不可撤销", "是否恢复出厂设置？"))
                {
                    if (GUI::msgbox_yn("这是最后一次提示", "将格式化nvs和LittleFS存储区", "取消（右）", "确认（左）") == false)
                    {
                        display.clearScreen();
                        u8g2Fonts.drawUTF8(30, 40, "正在格式化NVS存储");
                        display.display();
                        nvs_flash_erase();
                        display.clearScreen();
                        u8g2Fonts.drawUTF8(30, 40, "正在格式化LittleFS存储");
                        display.display(true);
                        LittleFS.end();
                        LittleFS.format();
                        display.clearScreen();
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