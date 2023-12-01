#include <A_Config.h>
#include <peripherals.h>
SPIClass SDSPI(HSPI);
Peripherals peripherals;
void Peripherals::check()
{
    display.clearScreen();
    GUI::drawWindowsWithTitle("重新检测外设", 0, 0, 296, 128);
    u8g2Fonts.drawUTF8(20, 30, "检测到硬件更改，正在重新检测外设");
    display.display();
    String msg = "";
    // 先检测I2C设备
    uint16_t i2cbitmask = 0;
    Wire.beginTransmission(AHTX0_I2CADDR_DEFAULT);
    if (Wire.endTransmission() == 0)
    {
        i2cbitmask |= PERIPHERALS_AHT20_BIT;
        msg += "AHT20温度传感器\n";
    }
    Wire.beginTransmission(BMP280_ADDRESS);
    if (Wire.endTransmission() == 0)
    {
        i2cbitmask |= PERIPHERALS_BMP280_BIT;
        msg += "BMP280气压传感器\n";
    }
    Wire.beginTransmission(SGP30_I2CADDR_DEFAULT);
    if (Wire.endTransmission() == 0)
    {
        i2cbitmask |= PERIPHERALS_SGP30_BIT;
        msg += "SGP30空气质量传感器\n";
    }
    Wire.beginTransmission(0x68);
    if (Wire.endTransmission() == 0)
    {
        i2cbitmask |= PERIPHERALS_DS3231_BIT;
        msg += "DS3231高精度RTC\n";
    }
    Serial.printf("Peripherals check OK: 0x%02x\n", i2cbitmask);
    Serial.println(msg);
    GUI::msgbox("检测到的外设", msg.c_str());
    i2cbitmask |= PERIPHERALS_SD_BIT;
    hal.pref.putUShort(SETTINGS_PARAM_PHERIPHERAL_BITMASK, i2cbitmask);
    peripherals_current = i2cbitmask;
    ESP.restart();
}

void Peripherals::init()
{
    Wire.begin(PIN_SDA, PIN_SCL);
    SDSPI.begin(PIN_SD_SCLK, PIN_SD_MISO, PIN_SD_MOSI, -1);
    peripherals_current = hal.pref.getUShort(SETTINGS_PARAM_PHERIPHERAL_BITMASK, 0xffff);
    if (peripherals_current == 0xffff)
    {
        check();
    }
    if (peripherals_current & PERIPHERALS_SGP30_BIT && sgpInited == false)
    {
        if (!sgp.begin(&Wire, false))
        {
            Serial.println("Sensor not found :(");
            while (1)
                ;
        }
        Serial.print("Found SGP30 serial #");
        Serial.print(sgp.serialnumber[0], HEX);
        Serial.print(sgp.serialnumber[1], HEX);
        Serial.println(sgp.serialnumber[2], HEX);
        sgpInited = true;
    }
}

uint16_t Peripherals::checkAvailable(uint16_t bitmask)
{
    return (peripherals_current & bitmask) ^ bitmask;
}

bool Peripherals::load(uint16_t bitmask)
{
    Serial.printf("[外设] 外设加载：0x%x -> 0x%x，当前安装：0x%x\n", peripherals_load, bitmask, peripherals_current);
    if (bitmask & PERIPHERALS_SD_BIT && (peripherals_load & PERIPHERALS_SD_BIT) == 0)
    {
        // 需要加载TF卡
        // 首先测试TF卡是否存在
        if (digitalRead(PIN_SD_CARDDETECT) != 1)
        {
            Serial.println("[外设] 加载TF卡");
            digitalWrite(PIN_SDVDD_CTRL, 0);
            delay(50);
            if (SD.begin(PIN_SD_CS, SDSPI) == false)
            {
                delay(100);
                if (SD.begin(PIN_SD_CS, SDSPI) == false)
                {
                    GUI::msgbox("错误", "存在TF卡，但无法挂载");
                    SD.end();
                }
            }
        }
    }
    else if ((bitmask & PERIPHERALS_SD_BIT) == 0 && peripherals_load & PERIPHERALS_SD_BIT)
    {
        // 卸载TF卡
        Serial.println("[外设] 卸载TF卡");
        SD.end();
        delay(50);
        digitalWrite(PIN_SDVDD_CTRL, 1);
    }
    // 只有sgp和SD卡需要重新加载
    if (bitmask & PERIPHERALS_SGP30_BIT && peripherals_current & PERIPHERALS_SGP30_BIT == 0)
    {
        Serial.println("[外设] 加载SGP30");
        // 需要加载sgp
        sgp.IAQinit();
    }
    else if ((bitmask & PERIPHERALS_SGP30_BIT) == 0 && peripherals_current & PERIPHERALS_SGP30_BIT)
    {
        sgp.softReset();
    }
    // 之后检查有些可选外设
    //  尝试按需初始化外设
    if (bitmask & PERIPHERALS_AHT20_BIT && peripherals_current & PERIPHERALS_AHT20_BIT && ahtInited == false)
    {
        Serial.println("[外设] 首次加载AHT20");
        if (!aht.begin())
        {
            Serial.println("Could not find AHT? Check wiring");
            check();
        }
        ahtInited = true;
    }
    // if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) {
    if (bitmask & PERIPHERALS_BMP280_BIT && peripherals_current & PERIPHERALS_BMP280_BIT && bmpInited == false)
    {
        Serial.println("[外设] 首次加载BMP280");
        if (!bmp.begin())
        {
            Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                             "try a different address!"));
            check();
        }
        /* Default settings from datasheet. */
        bmp.setSampling(Adafruit_BMP280::MODE_FORCED,    /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,    /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,   /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,     /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_63); /* Standby time. */
        bmpInited = true;
    }
    // DS3231无需初始化
    peripherals_load = bitmask;
    return true;
}
void Peripherals::load_append(uint16_t bitmask)
{
    int tmp = peripherals_load;
    if(tmp | bitmask == tmp)
        return;
    peripherals.load(bitmask | tmp);
}

void Peripherals::sleep()
{
    gpio_deep_sleep_hold_en();
    if (peripherals_load & PERIPHERALS_SD_BIT)
    {
        SD.end();
        delay(50);
        digitalWrite(PIN_SDVDD_CTRL, 1);
    }
    if (peripherals_load & PERIPHERALS_SGP30_BIT)
    {
        sgp.softReset();
        delay(50);
    }
    // 这里不清除加载的设备，唤醒后自动重新加载
}

void Peripherals::wakeup()
{
    if (peripherals_load == 0)
        return;
    uint16_t tmp = peripherals_load;
    peripherals_load = 0;
    load(tmp);
}
