#include "A_Config.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <SPIFFSEditor.h>

// 下面是引用网页文件
#include "index.h"
#include "csss.h"
#include "jss.h"
#include "jss2.h"
#include "favicon.h"
#include "blockly.h"
#include "jss3.h"
////////////////////////////下面是lua部分//////////////////////////
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
TaskHandle_t lua_server_handle = NULL;
static bool serverRunning = false;
bool LuaRunning = false; // 全局变量，表示Lua服务器是否运行，用于防止调试时误退出
extern "C" void lua_printf(const char *format, ...)
{
    va_list argptr;
    va_start(argptr, format);
    char str[1024];
    vsprintf(str, format, argptr);
    if (serverRunning)
    {
        ws.textAll(str);
    }
    else
    {
        puts(str);
    }
    va_end(argptr);
}
static void task_lua_server(void *)
{
    LuaRunning = true;
    lua_execute("/littlefs/webtmp/main.lua");
    lua_printf("[Lua程序结束]\n");
    lua_server_handle = NULL;
    LuaRunning = false;
    vTaskDelete(NULL);
}

// lua默认上传到LittleFS根目录下的temp.lua
void luaExecuteHandler(AsyncWebServerRequest *request)
{
    closeLua();
    openLua();
    setPath("/littlefs/webtmp");
    xTaskCreate(task_lua_server, "lua_server", 40960, NULL, 0, &lua_server_handle);
    request->send(200, "text/plain", "OK");
}

void luaTerminateHandler(AsyncWebServerRequest *request)
{
    if (lua_server_handle != NULL)
    {
        vTaskDelete(lua_server_handle);
        lua_server_handle = NULL;
    }
    closeLua();
    ws.textAll("\n[Lua语言服务任务已被删除]\n");
    request->send(200, "text/plain", "OK");
    LuaRunning = false;
}

void rmrfHandler(AsyncWebServerRequest *request)
{
    if (request->hasArg("path"))
    {
        String path = request->arg("path");
        if (path == "")
        {
            request->send(500, "text/plain", "EER");
        }
        hal.rm_rf((String("/littlefs/") + path).c_str());
        request->send(200, "text/plain", "OK");
        return;
    }
    request->send(500, "text/plain", "EER");
}
void renameHandler(AsyncWebServerRequest *request)
{
    if (request->hasArg("path") && request->hasArg("new"))
    {
        String path = request->arg("path");
        String newpath = request->arg("new");
        if (LittleFS.rename(path, newpath))
        {
            request->send(200, "text/plain", "OK");
            return;
        }
    }
    request->send(500, "text/plain", "EER");
}
void mkdirHandler(AsyncWebServerRequest *request)
{
    if (request->hasArg("path"))
    {
        String path = request->arg("path");
        if (LittleFS.mkdir(path))
        {
            request->send(200, "text/plain", "OK");
            return;
        }
    }
    request->send(500, "text/plain", "EER");
}
bool myxcopy(const String path, const String newpath)
{
    std::list<String> filenames;
    File root, file;
    filenames.push_back(path);
    String tmp;
    while (filenames.empty() == false)
    {
        root = LittleFS.open(filenames.back());
        tmp = filenames.back();
        tmp.replace(path, newpath);
        filenames.pop_back();
        if (!root)
        {
            Serial.println("[文件] 无法打开目录");
            continue;
        }
        LittleFS.mkdir(tmp);
        file = root.openNextFile();
        while (file)
        {
            String name = file.name();
            if (file.isDirectory())
            {
                tmp = file.path();
                tmp.replace(path, newpath);
                LittleFS.mkdir(tmp);
                filenames.push_back(file.path());
            }
            else
            {
                // 复制文件
                tmp = file.path();
                tmp.replace(path, newpath);
                File newFile = LittleFS.open(tmp, "w");
                if (!newFile)
                {
                    // 打开失败
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
void createAppHandler(AsyncWebServerRequest *request)
{
    if (request->hasArg("name"))
    {
        String name = request->arg("name");
        if (name == "")
        {
            request->send(500, "text/plain", "EER");
        }
        if (name.endsWith(".app") == false)
            name += ".app";
        hal.rm_rf((String("/littlefs/") + name).c_str());
        String currentPath = "/" + name;
        if (myxcopy("/webtmp", currentPath))
        {
            request->send(200, "text/plain", "OK");
            return;
        }
    }
    request->send(500, "text/plain", "EER");
}
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_DATA)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        String msg = "";
        if (info->final && info->index == 0 && info->len == len)
        {
            // the whole message is in a single frame and we got all of it's data
            // Serial.printf("ws[%u]", client->id());
            if (info->opcode == WS_TEXT)
            {
                /*
                    for (size_t i = 0; i < info->len; i++)
                    {
                            msg += (char)data[i];
                    }
                    */
            }
        }
    }
}
static void sendreq(AsyncWebServerRequest *request, const char *mime, const uint8_t *name, unsigned int len)
{
    const char *buildTime = __DATE__ " " __TIME__ " GMT";
    if (request->header("If-Modified-Since").equals(buildTime))
    {
        request->send(304);
    }
    else
    {
        AsyncWebServerResponse *response = request->beginResponse_P(200, mime, name, len);
        response->addHeader("Content-Encoding", "gzip");
        response->addHeader("Last-Modified", buildTime);
        request->send(response);
    }
}
void beginWebServer()
{
    if (LittleFS.exists("/webtmp") == false)
    {
        LittleFS.mkdir("/webtmp");
    }
    server.onNotFound([](AsyncWebServerRequest *request)
                      {
        if(WiFi.softAPgetStationNum() != 0)
        {
            request->redirect("http://192.168.4.1");
        }
        else
        {
            request->send(404);
        } });
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "text/html", __web_index_html_gz, __web_index_html_gz_len); });
    server.on("/blockly", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "text/html", __web_Blockly_html_gz, __web_Blockly_html_gz_len); });
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "image/x-icon", __favicon_ico_gz, __favicon_ico_gz_len); });
    server.on("/css/csss.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "text/css", __web_css_csss_css_gz, __web_css_csss_css_gz_len); });
    server.on("/js/jss.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "application/javascript", __web_js_jss_js_gz, __web_js_jss_js_gz_len); });
    server.on("/js/jss2.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "application/javascript", __web_js_jss2_js_gz, __web_js_jss2_js_gz_len); });
    server.on("/js/jss3.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { sendreq(request, "application/javascript", __web_js_jss3_js_gz, __web_js_jss3_js_gz_len); });

    server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                                String message;
                                message += "IP地址: ";
                                message += WiFi.localIP().toString();
                                message += "<br/>MAC地址: ";
                                message += WiFi.macAddress();
                                message += "<br/>系统时间: ";
                                char strftime_buf[64];
                                strftime(strftime_buf, sizeof(strftime_buf), "%c", &hal.timeinfo);
                                message += strftime_buf;
                                message += "<br/>ADC电压：";
                                message += hal.VCC;
                                message += "<br/>USB";
                                message += hal.USBPluggedIn?"已插入":"未插入";
                                message += hal.isCharging?"<br/>正在充电":"<br/>未充电";
                                request->send(200, "text/plain", message); });

    server.on("/conf", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                Serial.println(request->getParam("json", true, false)->value());
                                deserializeJson(config, request->getParam(0)->value());
                                request->send(200, "text/plain", "OK");
                                hal.saveConfig(); });

    server.on("/poweroff", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                                request->send(200, "text/plain", "OK");
                                delay(50);
                                hal.powerOff(); });

    server.on("/config.json", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", config.as<String>()); });

    server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                                request->send(200, "text/plain", "OK");
                                delay(100);
                                ESP.restart(); });

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", String(ESP.getFreeHeap())); });

    server.addHandler(new SPIFFSEditor(LittleFS));
    server.on("/rundebug", HTTP_GET, luaExecuteHandler);
    server.on("/terminate", HTTP_GET, luaTerminateHandler);
    server.on("/rmrf", HTTP_POST, rmrfHandler);
    server.on("/mkdir", HTTP_POST, mkdirHandler);
    server.on("/rename", HTTP_POST, renameHandler);
    server.on("/createapp", HTTP_POST, createAppHandler);
    server.begin();
    Serial.println("HTTP server started");
    serverRunning = true;
}
void updateWebServer()
{
    ws.cleanupClients();
    delay(100);
}
