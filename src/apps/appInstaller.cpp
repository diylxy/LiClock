#include "AppManager.h"

class AppInstaller : public AppBase
{
private:
    /* data */
public:
    AppInstaller()
    {
        name = "installer";
        title = "APP管理";
        description = "Lua应用管理器";
        image = NULL;
        peripherals_requested = PERIPHERALS_SD_BIT;
        _showInList = false;
    }
    std::list<String> appNames;
    int getAppSize(const String path, bool fromTF = false);
    void getLocalApp();
    void loadApp(const String path);
    bool install(const String path);
    void remove(const String appName);
    void setup();
    void menu_local();
    void menu_tf();
    bool end = false;
};
static AppInstaller app;
static LuaAppWrapper app_lua; // 用于临时启动TF卡应用
bool file_exist(const char *path);

int AppInstaller::getAppSize(const String path, bool fromTF)
{
    std::list<String> filenames;
    File root, file;
    int totalSize = 0;
    filenames.push_back(path);
    while (filenames.empty() == false)
    {
        if (fromTF == false)
            root = LittleFS.open(filenames.back());
        else
            root = SD.open(filenames.back());
        filenames.pop_back();
        if (!root)
        {
            Serial.println("[文件] 无法打开目录");
            continue;
        }
        file = root.openNextFile();
        while (file)
        {
            String name = file.name();
            if (file.isDirectory())
            {
                filenames.push_back(file.path());
            }
            else
            {
                totalSize += file.size();
            }
            file.close();
            file = root.openNextFile();
        }
    }
    root.close();
    return totalSize;
}

void AppInstaller::getLocalApp()
{
    appNames.clear();
    File root, file;
    root = LittleFS.open("/");
    if (!root)
    {
        Serial.println("[文件] root未打开");
    }
    file = root.openNextFile();
    while (file)
    {
        String name = file.name();
        if (file.isDirectory())
        {
            if (name.endsWith(".app"))
            {
                appNames.push_back(file.name());
            }
        }
        file.close();
        file = root.openNextFile();
    }
    root.close();
}

void AppInstaller::loadApp(const String path) // 加载TF卡App
{
    String filename = path.substring(path.lastIndexOf('/') + 1);
    app_lua.initialize(filename, path);
    app_lua.init();
    app_lua.peripherals_requested = PERIPHERALS_SD_BIT;
    app_lua._showInList = false;
    app_lua._reentrant = false;
    appManager.gotoApp(&app_lua);
}
bool AppInstaller::install(const String path)
{
    std::list<String> filenames;
    File root, file;
    filenames.push_back(path);
    while (filenames.empty() == false)
    {
        root = SD.open(filenames.back());
        filenames.pop_back();
        if (!root)
        {
            Serial.println("[文件] 无法打开目录");
            continue;
        }
        LittleFS.mkdir(path);
        file = root.openNextFile();
        while (file)
        {
            String name = file.name();
            if (file.isDirectory())
            {
                LittleFS.mkdir(file.path());
                filenames.push_back(file.path());
            }
            else
            {
                // 复制文件
                File newFile = LittleFS.open(file.path(), "w");
                if (!newFile)
                {
                    // 打开失败
                    GUI::msgbox("安装失败", "无法写入文件，可能空间已满");
                    Serial.println("无法写入文件");
                    file.close();
                    root.close();
                    return false;
                }
                hal.copy(newFile, file);
                newFile.close();
                file.close();
            }
            file.close();
            file = root.openNextFile();
        }
    }
    root.close();
    return true;
}

void AppInstaller::remove(const String appName)
{
    hal.rm_rf((String("/littlefs/") + appName).c_str());
}

static const menu_item appMenu_main[] = {
    {NULL, "退出"},
    {NULL, "安装 App"},
    {NULL, "卸载 App"},
    {NULL, "文件系统信息"},
    {NULL, NULL},
};
static const menu_item appMenu_Local[] = {
    {NULL, "退出"},
    {NULL, "运行"},
    {NULL, "卸载"},
    {NULL, "App 信息"},
    {NULL, NULL},
};
static const menu_item appMenu_TF[] = {
    {NULL, "退出"},
    {NULL, "运行"},
    {NULL, "安装"},
    {NULL, "App 信息"},
    {NULL, NULL},
};

void AppInstaller::setup()
{
    while (end == false)
    {
        int res = GUI::menu("App管理器", appMenu_main);
        switch (res)
        {
        case 0:
            appManager.goBack();
            return;
            break;
        case 1:
            if (SD.cardSize() != 0)
                menu_tf();
            break;
        case 2:
            menu_local();
            break;
        case 3:
        {
            char buf[40];
            sprintf(buf, "已用：%d%%\n总空间：%d kB", LittleFS.usedBytes() * 100 / LittleFS.totalBytes(), LittleFS.totalBytes() / 1024);
            GUI::msgbox("文件系统信息", buf);
        }
        break;
        default:
            break;
        }
    }
    end = false;
}

void AppInstaller::menu_local()
{
    while (end == false)
    {
        getLocalApp();
        if (appNames.size() == 0)
        {
            GUI::msgbox("无App", "不存在本地App");
            break;
        }
        menu_item *appList = new menu_item[appNames.size() + 2];
        appList[0].title = "返回";
        appList[0].icon = NULL;
        int i = 1;
        std::list<String>::iterator it;
        for (it = appNames.begin(); it != appNames.end(); ++it)
        {
            appList[i].title = (*it).c_str();
            appList[i].icon = NULL;
            ++i;
        }
        appList[i].title = NULL;
        appList[i].icon = NULL;
        int appIdx = GUI::menu("请选择App", appList);
        if (appIdx == 0)
        {
            delete appList;
            break;
        }
        int op = GUI::menu(appList[appIdx].title, appMenu_Local);
        switch (op)
        {
        case 0:
            break;
        case 1:
            appManager.gotoApp(appList[appIdx].title);
            end = true;
            delete appList;
            return;
            break;
        case 2:
            if (GUI::msgbox_yn("请确认", "是否确认卸载？此操作不可逆") == true)
            {
                hal.rm_rf(("/littlefs/" + String(appList[appIdx].title)).c_str());
                GUI::msgbox("删除成功", "App卸载成功");
            }
            break;
        case 3:
        {
            char buf[40];
            sprintf(buf, "大小：%d 字节", getAppSize("/" + String(appList[appIdx].title), false));
            GUI::msgbox(appList[appIdx].title, buf);
        }
        break;
        default:
            break;
        }
        delete appList;
    }
}

void AppInstaller::menu_tf()
{
    while (end == false)
    {
        const char *appPath = GUI::fileDialog("请选择App", true);
        if (appPath == NULL)
            break;
        String tmp = appPath;
        String title = tmp.substring(tmp.lastIndexOf('/') + 1);
        int res = GUI::menu(title.c_str(), appMenu_TF);
        switch (res)
        {
        case 0:
            break;
        case 1:
            loadApp(appPath);
            end = true;
            return;
            break;
        case 2:
            tmp.replace("/sd", "");
            if (install(tmp))
            {
                GUI::msgbox("安装成功", "App安装完成，重启后加载");
            }
            else
            {
                GUI::msgbox("安装失败", "App安装失败，请检查可用空间是否充足");
            }
            return;
            break;
        case 3:
            tmp.replace("/sd", "");
            {
                char buf[30];
                sprintf(buf, "大小：%d 字节", getAppSize(tmp, true));
                GUI::msgbox(title.c_str(), buf);
            }
        default:
            break;
        }
    }
}
