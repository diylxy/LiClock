#include <A_Config.h>
#include "AppManager.h"

class AppBilibili : public AppBase
{
private:
    /* data */
public:
    AppBilibili()
    {
        name = "bilibili";
        title = "B站粉丝";
        description = "简易Bilibili粉丝数显示器";
        image = NULL;
    }
    void setup();
};
static AppBilibili app;

String UID;
DynamicJsonDocument doc(4096);
String Response;
String Cookies;
String Headers;
int HeaderCount;
JsonObject obj;
///////////////////////////////////////////////////////
// 从cookie中获取UID
static void updateUID()
{
    uint16_t i = Cookies.indexOf("DedeUserID=") + 11;
    if (i == -1)
    {
        UID = "";
        return;
    }
    UID = Cookies.substring(i, Cookies.indexOf(';', i));
}

static String HTTPGET(const char *url)
{
    HTTPClient http;

    String payload = "";
    for (uint8_t i = 0; i < 10; ++i)
    {
        if (WiFi.status() != WL_CONNECTED)
            return "";
        if (http.begin(url))
        {
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            http.addHeader("Referer", "https://www.bilibili.com");
            if (Cookies != "")
                http.addHeader("Cookie", Cookies);
            if (http.GET() == HTTP_CODE_OK)
            {
                payload = http.getString();
                http.end();
                return payload;
            }
        }
        vTaskDelay(100);
    }
    return payload;
}

static bool checkCookies()
{
    HTTPClient http;
    String testURL = "https://api.bilibili.com/x/relation/followers?vmid=" + UID + "&ps=1&pn=6";
    if (http.begin(testURL))
    {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        http.addHeader("Referer", "https://www.bilibili.com");
        http.addHeader("Cookie", Cookies);
        if (http.GET() == HTTP_CODE_OK)
        {
            Response = http.getString();
        }
        http.end();
    }
    ArduinoJson::deserializeJson(doc, Response);
    obj = doc.as<JsonObject>();
    if (obj["code"] == 0)
    {
        return true;
    }
    return false;
}

static bool login()
{
    String URL = "";
    String oauthKey;
    String Response;
    uint32_t millisStart;
    display.clearScreen();
    u8g2Fonts.setCursor(30, 30);
    u8g2Fonts.print("正在获取二维码");
    display.display();
    while (1)
    {
    RebuildQRCode:
        millisStart = millis();
        Response = HTTPGET("https://passport.bilibili.com/x/passport-login/web/qrcode/generate");
        if (Response == "")
        {
            GUI::msgbox("错误", "二维码生成失败，请联系开发者");
            return false;
        }
        Serial.println(Response);
        ArduinoJson::deserializeJson(doc, Response);
        obj = doc.as<JsonObject>();

        URL = obj["data"]["url"].as<String>();
        oauthKey = obj["data"]["qrcode_key"].as<String>();
        display.clearScreen();
        QRCode qrcode;
        uint8_t qrcodeData[qrcode_getBufferSize(7)];
        qrcode_initText(&qrcode, qrcodeData, 6, 0, URL.c_str());
        Serial.println(qrcode.size);
        for (uint8_t y = 0; y < qrcode.size; y++)
        {
            // Each horizontal module
            for (uint8_t x = 0; x < qrcode.size; x++)
            {
                display.fillRect(2 * x + 20, 2 * y + 20, 2, 2, qrcode_getModule(&qrcode, x, y) ? GxEPD_BLACK : GxEPD_WHITE);
            }
        }
        display.display(true);
        while (millis() - millisStart < 180000)
        {
            Serial.print(".");
            HTTPClient http;
            if (hal.btnl.isPressing())
            {
                Serial.println("cancel");
                goto err;
            }
            vTaskDelay(1000);
            if (http.begin("https://passport.bilibili.com/x/passport-login/web/qrcode/poll" + String("?qrcode_key=") + oauthKey))
            {
                http.addHeader("Content-Type", "application/x-www-form-urlencoded");
                http.addHeader("Referer", "https://www.bilibili.com");
                http.addHeader("User-Agent", "curl/7.70.0");
                int code = http.GET();
                if (code == HTTP_CODE_OK)
                {
                    Response = http.getString();
                }
                else
                {
                    Serial.println(code);//307
                    Serial.println(http.header("Location"));
                }
            }

            ArduinoJson::deserializeJson(doc, Response);
            obj = doc.as<JsonObject>();

            if (obj["data"]["code"] == 0)
            {
                Response = obj["data"]["url"].as<String>();
                Cookies = Response.substring(Response.indexOf("?") + 1);
                Cookies.replace("&", ";");
                http.end();
                Serial.println("ok");
                goto scaned;
            }
            else
            {
                if (obj["data"]["code"] == 86038)
                {
                    GUI::msgbox("超时", "二维码超时");
                    http.end();
                    goto RebuildQRCode;
                }
                else if (obj["data"]["code"] == 86090)
                {
                    Serial.print("*");
                }
            }
            http.end();
        }
        continue;
    err:
        break;
    }
scaned:
    updateUID();
    if (checkCookies())
    {
        hal.pref.putString("cookies", Cookies);
        GUI::msgbox("成功", "二维码登录成功");
        return true;
    }
    else
    {
        GUI::msgbox("失败", "二维码登录失败");
        return false;
    }
}

static void updateInfo()
{
    display.clearScreen();
    uint16_t like;
    uint16_t reply;
    uint16_t msg;
    Response = HTTPGET("https://api.bilibili.com/x/msgfeed/unread");
    ArduinoJson::deserializeJson(doc, Response);
    obj = doc.as<JsonObject>();
    if (obj["code"] != 0)
    {
        // 获取消息数失败
        Serial.println(Response);
        GUI::msgbox("错误", "获取消息数失败");
        return;
    }
    like = obj["data"]["like"];
    reply = obj["data"]["reply"];
    Response = HTTPGET("https://api.vc.bilibili.com/session_svr/v1/session_svr/single_unread");
    ArduinoJson::deserializeJson(doc, Response);
    obj = doc.as<JsonObject>();
    if (obj["code"] != 0)
    {
        // 获取私信数失败
        Serial.println(Response);
        GUI::msgbox("错误", "获取私信数失败");
        return;
    }
    msg = obj["data"]["unfollow_unread"].as<int>() + obj["data"]["follow_unread"].as<int>();
    u8g2Fonts.setCursor(30, 30);
    u8g2Fonts.printf("点赞：%d\n回复：%d\n私信：%d", like, reply, msg);
    // 关注数
    Response = HTTPGET(("https://api.bilibili.com/x/relation/stat?jsonp=jsonp&vmid=" + UID).c_str());
    ArduinoJson::deserializeJson(doc, Response);
    obj = doc.as<JsonObject>();
    if (obj["code"] != 0)
    {
        // 获取关注数失败
        Serial.println(Response);
        GUI::msgbox("错误", "获取粉丝数失败");
        return;
    }
    int fans_now = obj["data"]["follower"];
    u8g2Fonts.printf("粉丝数: %d\n", fans_now);
    display.display();
}

void AppBilibili::setup()
{
    Cookies = hal.pref.getString("cookies", "");
    hal.autoConnectWiFi();
    updateUID();
    if (checkCookies() == false)
    {
        if (login() == false)
        {
            appManager.goBack();
            return;
        }
    }
    updateInfo();
    appManager.nextWakeup = 300;
    WiFi.disconnect();
}
