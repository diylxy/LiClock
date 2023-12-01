#include <A_Config.h>
#include <GUI.h>
// 9 * 12
static const uint8_t imgfile_bits[] = {
    0x7e, 0x00, 0xa1, 0x00, 0x21, 0x01, 0xc9, 0x01, 0x15, 0x01, 0x09, 0x01,
    0x01, 0x01, 0x41, 0x01, 0xe1, 0x01, 0xf1, 0x01, 0xfd, 0x01, 0xff, 0x01};
static const uint8_t luafile_bits[] = {
    0x7e, 0x00, 0xa1, 0x00, 0x21, 0x01, 0xc1, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x21, 0x01, 0x09, 0x01, 0x15, 0x01, 0x09, 0x01, 0x01, 0x01, 0xff, 0x01};
static const uint8_t musicfile_bits[] = {
    0x7e, 0x00, 0xa1, 0x00, 0x21, 0x01, 0xc1, 0x01, 0x01, 0x01, 0x61, 0x01,
    0x59, 0x01, 0x49, 0x01, 0x69, 0x01, 0x0d, 0x01, 0x01, 0x01, 0xff, 0x01};
static const uint8_t otherfile_bits[] = {
    0x7e, 0x00, 0xa1, 0x00, 0x21, 0x01, 0xc1, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xff, 0x01};
static const uint8_t sysfile_bits[] = {
    0x7e, 0x00, 0xa1, 0x00, 0x21, 0x01, 0xc1, 0x01, 0x01, 0x01, 0x11, 0x01,
    0x11, 0x01, 0x11, 0x01, 0x01, 0x01, 0x11, 0x01, 0x01, 0x01, 0xff, 0x01};
static const uint8_t textfile_bits[] = {
    0x7e, 0x00, 0xa1, 0x00, 0x21, 0x01, 0xc1, 0x01, 0x01, 0x01, 0x7d, 0x01,
    0x01, 0x01, 0x7d, 0x01, 0x01, 0x01, 0x7d, 0x01, 0x01, 0x01, 0xff, 0x01};
// 12*12
static const uint8_t folder_bits[] = {
    0x00, 0x00, 0x1e, 0x00, 0x21, 0x00, 0xc1, 0x0f, 0x01, 0x08, 0x01, 0x08,
    0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0x01, 0x08, 0xfe, 0x07, 0x00, 0x00};

struct s_fileicondict
{
    const char *extension;
    const uint8_t *ico;
};
static const struct s_fileicondict fileicondict[] = {
    {"bmp", imgfile_bits},
    {"jpg", imgfile_bits},
    {"png", imgfile_bits},
    {"xbm", imgfile_bits},
    {"lua", luafile_bits},
    {"buz", musicfile_bits},
    {"json", sysfile_bits},
    {"bin", sysfile_bits},
    {"txt", textfile_bits},
    {"lbm", imgfile_bits}, // 自定义的图片格式（实际上是XBM编码）
    {NULL, NULL},
};
/**
 *
“lbm”格式定义：
扩展名.lbm
单色位图
小端
2字节宽度
2字节高度
之后是数据
*/
static const uint8_t *getFileIcon(const char *extension)
{
    for (int i = 0; fileicondict[i].extension != NULL; i++)
    {
        if (strcmp(fileicondict[i].extension, extension) == 0)
        {
            return fileicondict[i].ico;
        }
    }
    return otherfile_bits;
}
namespace GUI
{
    char filedialog_buffer[300];
    void push_buffer();
    void pop_buffer();
    // 注意，下面这个函数完全没有考虑线程安全，no reentrent!!!
    const char *fileDialog(const char *title, bool isApp, const char *endsWidth)
    {
        // 首先选择文件系统
        bool useSD = false;
        if (isApp == false)
        {
            if (peripherals.isSDLoaded())
            {
                if (msgbox_yn("请选择文件系统", "左：LittleFS\n右：TF 卡", "TF 卡", "LittleFS"))
                {
                    useSD = true;
                }
            }
        }
        else
        {
            useSD = true;
        }
        String cwd = "/";
        File root;
        File file;
        int16_t total_entries = 0;
        menu_item entries[128];
        char *titles[128];
        memset(entries, 0, sizeof(entries));
        memset(titles, 0, sizeof(titles));
        entries[0].icon = folder_bits;
        entries[0].title = "..";
        while (1)
        {
            Serial.print("[文件浏览] 当前工作目录：");
            Serial.println(cwd);
            // 首先清除
            total_entries = 1;
            while (titles[total_entries] != NULL)
            {
                free(titles[total_entries]);
                titles[total_entries] = NULL;
                entries[total_entries].icon = NULL;
                entries[total_entries].title = NULL;
                ++total_entries;
            }
            total_entries = 1;
            if (useSD)
            {
                root = SD.open(cwd);
            }
            else
            {
                root = LittleFS.open(cwd);
            }
            if (cwd != "/")
                cwd += "/";
            if (!root)
            {
                Serial.println("[文件] root未打开");
            }
            file = root.openNextFile();
            while (file)
            {
                String tmp = file.name();
                String ext;
                if (tmp.lastIndexOf('.') != -1)
                {
                    ext = tmp.substring(tmp.lastIndexOf('.') + 1);
                }
                else
                {
                    ext = "";
                }
                Serial.println(tmp);
                if (tmp.endsWith(".i"))
                {
                    file.close();
                    file = root.openNextFile();
                    continue;
                }
                if (isApp == false)
                {
                    if (file.isDirectory())
                    {
                        entries[total_entries].icon = folder_bits;
                    }
                    else
                    {
                        if (endsWidth != NULL)
                        {
                            if (strcmp(endsWidth, ext.c_str()) != 0)
                            {
                                file.close();
                                file = root.openNextFile();
                                continue;
                            }
                        }
                        entries[total_entries].icon = getFileIcon(ext.c_str());
                    }
                    titles[total_entries] = (char *)malloc(strlen(file.name()) + 1);
                    if (titles[total_entries] == NULL)
                    {
                        GUI::msgbox("严重错误", "[文件管理] 动态内存不足");
                        ESP.restart();
                    }
                    strcpy(titles[total_entries], file.name());
                    entries[total_entries].title = titles[total_entries];
                    ++total_entries;
                }
                else
                {
                    if (file.isDirectory())
                    {
                        if (ext == "app")
                        {
                            entries[total_entries].icon = luafile_bits;
                            titles[total_entries] = (char *)malloc(strlen(file.name()) + 1);
                            if (titles[total_entries] == NULL)
                            {
                                GUI::msgbox("严重错误", "[文件管理] 动态内存不足");
                                ESP.restart();
                            }
                            strcpy(titles[total_entries], file.name());
                            entries[total_entries].title = titles[total_entries];
                            ++total_entries;
                        }
                    }
                }
                if (total_entries >= 128)
                {
                    GUI::msgbox("严重错误", "文件数超过128个，即将重启");
                    ESP.restart();
                }
                file.close();
                file = root.openNextFile();
            }
            int selected = menu(title, entries, 12, 12);
            if (selected == 0)
            {
                // 上一级目录
                display.display(false); // 全局刷新一次
                if (cwd == "/")
                {
                    total_entries = 1;
                    while (titles[total_entries] != NULL)
                    {
                        free(titles[total_entries]);
                        titles[total_entries] = NULL;
                        ++total_entries;
                    }
                    return NULL;
                }
                else
                {
                    if (cwd.lastIndexOf('/', cwd.length() - 2) == 0)
                    {
                        // 下一个是根目录
                        cwd = "/";
                        root.close();
                    }
                    else
                    {
                        cwd = cwd.substring(0, cwd.lastIndexOf('/', cwd.length() - 2));
                        root.close();
                    }
                    continue;
                }
            }
            else if (entries[selected].icon == folder_bits)
            {
                // 是文件夹
                cwd += entries[selected].title;
                root.close();
                continue;
            }
            else
            {
                sprintf(filedialog_buffer, "%s%s%s", useSD ? "/sd" : "/littlefs", cwd.c_str(), entries[selected].title);
                break;
            }
        }
        total_entries = 1;
        while (titles[total_entries] != NULL)
        {
            free(titles[total_entries]);
            titles[total_entries] = NULL;
            ++total_entries;
        }
        return filedialog_buffer;
    }
}
