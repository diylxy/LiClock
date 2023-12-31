#include <A_Config.h>

float rain_data_raw[SAMPLE_COUNT];
int ydata[SCREEN_WIDTH + PX_PER_SAMPLE];

// 余弦
float cosineInterpolation(float point)
{
    float result = 0.0;

    // 寻找插值区间
    int leftIndex = (int)floor(point);
    int rightIndex = leftIndex + 1;

    // 计算权重
    float weight = (1 - cos((point - leftIndex) * M_PI)) / 2;

    // 执行余弦插值计算
    result = (1 - weight) * rain_data_raw[leftIndex] + weight * rain_data_raw[rightIndex];

    return result;
}

// 对降水数据进行插值
void processRain(float max = 300.0)
{
    float factor = (float)(SAMPLE_COUNT/SAMPLE_STEP - 2) / (float)(SCREEN_WIDTH);
    int i;
    for (i = 0; i < SCREEN_WIDTH + PX_PER_SAMPLE; i++)
    {
        ydata[i] = (int)(cosineInterpolation(((float)i) * factor) * GRAPH_HEIGHT / max);
    }
}
// 下面是天气图标部分
static const unsigned char sun_bits[] = {
    0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0x98, 0x01, 0x00, 0xf0, 0x9c, 0xf3, 0x00, 0xf0, 0x0f, 0xff, 0x00,
    0x30, 0x06, 0xc6, 0x00, 0x30, 0x00, 0xc0, 0x00, 0x20, 0xf8, 0x41, 0x00,
    0x60, 0xfc, 0x63, 0x00, 0x70, 0x06, 0xe6, 0x00, 0x38, 0x03, 0xcc, 0x01,
    0x1c, 0x03, 0x8c, 0x03, 0x07, 0x03, 0x0c, 0x0e, 0x07, 0x03, 0x0c, 0x0e,
    0x1c, 0x03, 0x8c, 0x03, 0x38, 0x03, 0xcc, 0x01, 0x60, 0x06, 0x66, 0x00,
    0x60, 0xfc, 0x63, 0x00, 0x20, 0xf8, 0x41, 0x00, 0x30, 0x00, 0xc0, 0x00,
    0x30, 0x06, 0xc6, 0x00, 0xf0, 0x0f, 0xff, 0x00, 0xf0, 0x98, 0xf1, 0x00,
    0x00, 0x98, 0x01, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
    0x00, 0x60, 0x00, 0x00};
static const unsigned char cloud_sun_bits[] = {
    0x00, 0x02, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x84, 0x8d, 0x00, 0x00,
    0xf8, 0x78, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x18, 0x43, 0x00, 0x00,
    0xd8, 0x4d, 0x00, 0x00, 0x4c, 0xd0, 0x00, 0x00, 0x26, 0x40, 0x00, 0x00,
    0x66, 0xfc, 0x01, 0x00, 0x4c, 0x06, 0x3f, 0x00, 0x58, 0x02, 0x60, 0x00,
    0x98, 0x02, 0x40, 0x00, 0x08, 0x03, 0xc0, 0x01, 0x98, 0x03, 0x80, 0x07,
    0xe4, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x0c, 0x40, 0x00, 0x00, 0x0c,
    0x40, 0x00, 0x00, 0x0c, 0xc0, 0x00, 0x00, 0x04, 0x80, 0x03, 0x00, 0x07,
    0x00, 0xff, 0xff, 0x01};

static const unsigned char cloud_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00,
    0x80, 0x81, 0x01, 0x00, 0xc0, 0x00, 0x3f, 0x00, 0x60, 0x00, 0xfe, 0x00,
    0x20, 0x00, 0x80, 0x00, 0x20, 0x00, 0x80, 0x01, 0x30, 0x00, 0x00, 0x01,
    0x38, 0x00, 0x00, 0x01, 0x0c, 0x00, 0x80, 0x01, 0x06, 0x00, 0x00, 0x03,
    0x02, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0x0c,
    0x03, 0x00, 0x00, 0x0c, 0x02, 0x00, 0x00, 0x0c, 0x06, 0x00, 0x00, 0x04,
    0x0c, 0x00, 0x00, 0x07, 0xf8, 0xff, 0xff, 0x03, 0xe0, 0xff, 0x7f, 0x00,
    0x00, 0x00, 0x00, 0x00};

static const unsigned char cloud_rain_bits[] = {
    0x00, 0x0f, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0xe0, 0xe0, 0x0f, 0x00,
    0x30, 0xc0, 0x3f, 0x00, 0x30, 0xc0, 0x30, 0x00, 0x10, 0x00, 0x60, 0x00,
    0x18, 0x00, 0xe0, 0x00, 0x1c, 0x00, 0xe0, 0x03, 0x0e, 0x00, 0x00, 0x07,
    0x06, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x0c,
    0x03, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x0c, 0x06, 0x00, 0x00, 0x06,
    0x0e, 0x00, 0x00, 0x07, 0xf8, 0xff, 0xff, 0x01, 0xc0, 0xff, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x60, 0x40, 0x00, 0x70, 0x60, 0xe0, 0x00,
    0x78, 0xf0, 0xe0, 0x01, 0xd8, 0xf0, 0xb0, 0x01, 0xcc, 0x98, 0x31, 0x03,
    0x8c, 0x98, 0x11, 0x03, 0xcc, 0x98, 0x31, 0x03, 0xf8, 0xf8, 0xf1, 0x01,
    0x70, 0xf0, 0xe0, 0x00};

static const unsigned char cloud_showers_bits[] = {
    0x00, 0x0f, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0xe0, 0xe0, 0x0f, 0x00,
    0x30, 0xc0, 0x3f, 0x00, 0x30, 0xc0, 0x30, 0x00, 0x10, 0x00, 0x60, 0x00,
    0x18, 0x00, 0xe0, 0x00, 0x1c, 0x00, 0xe0, 0x03, 0x0e, 0x00, 0x00, 0x07,
    0x06, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x0c,
    0x03, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x0c, 0x06, 0x00, 0x00, 0x06,
    0x0e, 0x00, 0x00, 0x07, 0xf8, 0xff, 0xff, 0x01, 0xc0, 0xff, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x20, 0x80, 0x00,
    0x0c, 0x60, 0x80, 0x01, 0x8c, 0x60, 0x84, 0x01, 0x8c, 0x61, 0x8c, 0x01,
    0x8c, 0x61, 0x8c, 0x01, 0x84, 0x21, 0x8c, 0x00, 0x80, 0x01, 0x0c, 0x00,
    0x80, 0x00, 0x04, 0x00};

static const unsigned char fog_bits[] = {
    0x00, 0x1e, 0x00, 0x00, 0x80, 0x33, 0x03, 0x00, 0x80, 0xc0, 0x0f, 0x00,
    0xc0, 0x40, 0x08, 0x00, 0x40, 0x00, 0x18, 0x00, 0x60, 0x00, 0x78, 0x00,
    0x30, 0x00, 0xc0, 0x00, 0x18, 0x00, 0x80, 0x01, 0x18, 0x00, 0x80, 0x01,
    0x08, 0x00, 0x00, 0x01, 0x18, 0x00, 0x80, 0x01, 0x10, 0x00, 0x80, 0x00,
    0xf0, 0xff, 0xff, 0x00, 0xc0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xf8, 0xe3, 0xff, 0x0f};

static const unsigned char snowflake_bits[] = {
    0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x93, 0x01, 0x00,
    0x00, 0xfe, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x10, 0x38, 0x10, 0x00,
    0x30, 0x10, 0x18, 0x00, 0x37, 0x10, 0xd8, 0x01, 0x3e, 0x10, 0xf8, 0x00,
    0x78, 0x10, 0x3c, 0x00, 0xfe, 0x10, 0xfe, 0x00, 0xcf, 0x93, 0xe7, 0x01,
    0x00, 0xff, 0x01, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00,
    0x00, 0xff, 0x01, 0x00, 0xcf, 0x93, 0xe7, 0x01, 0xfe, 0x10, 0xfe, 0x00,
    0x78, 0x10, 0x3c, 0x00, 0x3e, 0x10, 0xf8, 0x00, 0x37, 0x10, 0xd8, 0x01,
    0x30, 0x10, 0x18, 0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x7c, 0x00, 0x00,
    0x00, 0xfe, 0x00, 0x00, 0x00, 0x93, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x10, 0x00, 0x00};

static const unsigned char sun_dust_bits[] = {
    0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0x98, 0x01, 0x00, 0xf0, 0x98, 0x7b, 0x00, 0xf0, 0x0f, 0x3f, 0x00,
    0x30, 0x06, 0x06, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x20, 0xf8, 0x01, 0x0c,
    0x60, 0xfc, 0x03, 0x00, 0x60, 0x06, 0x00, 0x00, 0x38, 0x03, 0x00, 0x00,
    0x1c, 0x03, 0x60, 0x00, 0x07, 0x03, 0x60, 0x00, 0x07, 0x03, 0x00, 0x00,
    0x1c, 0x03, 0x00, 0x00, 0x38, 0x03, 0x02, 0x04, 0x70, 0x02, 0x07, 0x0c,
    0x60, 0x00, 0x02, 0x04, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x30, 0x30, 0x60, 0x00, 0x10, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x07, 0x0c,
    0x80, 0x01, 0x02, 0x0c};

static const unsigned char thunderstorm_bits[] = {
    0x00, 0x0f, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0xe0, 0xe0, 0x0f, 0x00,
    0x30, 0xc0, 0x3f, 0x00, 0x30, 0xc0, 0x30, 0x00, 0x10, 0x00, 0x60, 0x00,
    0x18, 0x00, 0xe0, 0x00, 0x1c, 0x00, 0xe0, 0x03, 0x0e, 0x00, 0x00, 0x07,
    0x06, 0xfc, 0x01, 0x06, 0x03, 0xfe, 0x01, 0x0c, 0x03, 0x86, 0x01, 0x0c,
    0x03, 0x86, 0x01, 0x0c, 0x03, 0xc6, 0x00, 0x0c, 0x06, 0xc6, 0x1f, 0x06,
    0x0e, 0xc2, 0x9f, 0x07, 0x78, 0x03, 0xcc, 0x01, 0x00, 0x03, 0x0c, 0x00,
    0x00, 0x3f, 0x06, 0x00, 0x00, 0x7f, 0x06, 0x00, 0x00, 0x60, 0x03, 0x00,
    0x00, 0x60, 0x03, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xe0, 0x01, 0x00,
    0x00, 0xe0, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
    0x00, 0x60, 0x00, 0x00};

static const unsigned char wind_bits[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00,
    0x00, 0x80, 0x7f, 0x00, 0x00, 0x80, 0xc1, 0x00, 0x00, 0xc0, 0xc0, 0x00,
    0x00, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xc0, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x78, 0x00, 0xff, 0xff, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc7, 0xff, 0x00,
    0xff, 0x8f, 0xff, 0x03, 0x00, 0x18, 0x00, 0x07, 0x00, 0x30, 0x00, 0x06,
    0x00, 0x30, 0x00, 0x0c, 0x10, 0x30, 0x00, 0x0c, 0x30, 0x30, 0x00, 0x0c,
    0x30, 0x18, 0x03, 0x0c, 0xe0, 0x1c, 0x03, 0x04, 0xc0, 0x0f, 0x06, 0x06,
    0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00};
static const unsigned char moon_bits[] = {
   0x00, 0xf0, 0x03, 0x00, 0x00, 0xfe, 0x07, 0x00, 0x80, 0x07, 0x06, 0x00,
   0xc0, 0x01, 0x03, 0x00, 0xe0, 0x80, 0x01, 0x00, 0x70, 0xc0, 0x00, 0x00,
   0x30, 0x60, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00,
   0x0c, 0x30, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x04, 0x30, 0x00, 0x00,
   0x04, 0x30, 0x00, 0x00, 0x06, 0x30, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00,
   0x04, 0x60, 0x00, 0x00, 0x04, 0xc0, 0x00, 0x00, 0x0c, 0xc0, 0x01, 0x00,
   0x0c, 0x80, 0x03, 0x00, 0x08, 0x00, 0x0f, 0x00, 0x18, 0x00, 0xfc, 0x03,
   0x30, 0x00, 0xf0, 0x07, 0x70, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x80, 0x03,
   0xc0, 0x01, 0xc0, 0x01, 0x80, 0x07, 0x70, 0x00, 0x00, 0xfe, 0x3f, 0x00,
   0x00, 0xf0, 0x07, 0x00 };
static const unsigned char cloud_moon_bits[] = {
   0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x6f, 0x00, 0x00, 0x80, 0x31, 0x00,
   0x00, 0xc0, 0x10, 0x00, 0x00, 0x40, 0x18, 0x00, 0x00, 0x60, 0x08, 0x00,
   0x00, 0x20, 0x08, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x20, 0x18, 0x00,
   0x00, 0x00, 0x10, 0x00, 0xe0, 0x1b, 0x30, 0x00, 0x30, 0x3e, 0xe0, 0x00,
   0x18, 0x60, 0x80, 0x0f, 0x08, 0xc0, 0x00, 0x06, 0x0c, 0xc0, 0x03, 0x03,
   0x06, 0x00, 0xf6, 0x01, 0x03, 0x00, 0x04, 0x00, 0x03, 0x00, 0x04, 0x00,
   0x03, 0x00, 0x04, 0x00, 0x02, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x03, 0x00,
   0xf8, 0xff, 0x01, 0x00 };

const ico_desc weather_icons_day[] = {
    {sun_bits, 28, 28},
    {moon_bits, 28, 28},
    {cloud_sun_bits, 28, 22},
    {cloud_moon_bits, 28, 22},
    {cloud_bits, 28, 22},
    {cloud_bits, 28, 22},
    {fog_bits, 28, 22},
    {fog_bits, 28, 22},
    {cloud_rain_bits, 28, 28},
    {cloud_rain_bits, 28, 28},
    {cloud_showers_bits, 28, 28},
    {cloud_showers_bits, 28, 28},
    {fog_bits, 28, 22},
    {snowflake_bits, 25, 28},
    {snowflake_bits, 25, 28},
    {snowflake_bits, 25, 28},
    {snowflake_bits, 25, 28},
    {sun_dust_bits, 28, 28},
    {sun_dust_bits, 28, 28},
    {wind_bits, 28, 28},
    //{thunderstorm_bits, 28, 28},
    };
