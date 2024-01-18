// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Controller: IL3820 : http://www.e-paper-display.com/download_detail/downloadsId=540.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_290.h"

GxEPD2_290::GxEPD2_290(int16_t cs, int16_t dc, int16_t rst, int16_t busy) : GxEPD2_EPD(cs, dc, rst, busy, HIGH, 10000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}
void GxEPD2_290::__clearScreen(uint8_t value)
{
  _initial_write = false; // initial full screen buffer clean done
  if (_initial_refresh)
  {
    _Init_Full();
    _setPartialRamArea(0, 0, WIDTH, HEIGHT);
    _writeCommand(0x24);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
      _writeData(value);
    }
    _Update_Full();
    _initial_refresh = false; // initial full update done
  }
  else
  {
    if (!_using_partial_mode)
      _Init_Part();
    _setPartialRamArea(0, 0, WIDTH, HEIGHT);
    _writeCommand(0x24);
    for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
    {
      _writeData(value);
    }
    _Update_Part();
  }
  if (!_using_partial_mode)
    _Init_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x24);
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _writeData(value);
  }
  _Update_Part();
}

void GxEPD2_290::__writeScreenBuffer(uint8_t value)
{
  _initial_write = false; // initial full screen buffer clean done
  // this controller has no command to write "old data"
  if (_initial_refresh)
    __clearScreen(value);
  else
    _writeScreenBuffer(value);
}

void GxEPD2_290::_writeScreenBuffer(uint8_t value)
{
  if (!_using_partial_mode)
    _Init_Part();
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x24);
  _startTransfer();
  for (uint32_t i = 0; i < uint32_t(WIDTH) * uint32_t(HEIGHT) / 8; i++)
  {
    _transfer(value);
  }
  _endTransfer();
}

void GxEPD2_290::__writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write)
    __writeScreenBuffer();                                        // initial full screen buffer clean
  delay(1);                                                       // yield() to avoid WDT on ESP8266 and ESP32
  int16_t wb = (w + 7) / 8;                                       // width bytes, bitmaps are padded
  x -= x % 8;                                                     // byte boundary
  w = wb * 8;                                                     // byte boundary
  int16_t x1 = x < 0 ? 0 : x;                                     // limit
  int16_t y1 = y < 0 ? 0 : y;                                     // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x;   // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0))
    return;
  if (!_using_partial_mode)
    _Init_Part();
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x24);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb, h of bitmap for index!
      int16_t idx = mirror_y ? j + dx / 8 + ((h - 1 - (i + dy))) * wb : j + dx / 8 + (i + dy) * wb;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert)
        data = ~data;
      _transfer(data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_290::__writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                  int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write)
    __writeScreenBuffer(); // initial full screen buffer clean
  delay(1);                // yield() to avoid WDT on ESP8266 and ESP32
  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0))
    return;
  if ((x_part < 0) || (x_part >= w_bitmap))
    return;
  if ((y_part < 0) || (y_part >= h_bitmap))
    return;
  int16_t wb_bitmap = (w_bitmap + 7) / 8;                         // width bytes, bitmaps are padded
  x_part -= x_part % 8;                                           // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w;              // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h;              // limit
  x -= x % 8;                                                     // byte boundary
  w = 8 * ((w + 7) / 8);                                          // byte boundary, bitmaps are padded
  int16_t x1 = x < 0 ? 0 : x;                                     // limit
  int16_t y1 = y < 0 ? 0 : y;                                     // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x;   // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0))
    return;
  if (!_using_partial_mode)
    _Init_Part();
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(0x24);
  _startTransfer();
  for (int16_t i = 0; i < h1; i++)
  {
    for (int16_t j = 0; j < w1 / 8; j++)
    {
      uint8_t data;
      // use wb_bitmap, h_bitmap of bitmap for index!
      int16_t idx = mirror_y ? x_part / 8 + j + dx / 8 + ((h_bitmap - 1 - (y_part + i + dy))) * wb_bitmap : x_part / 8 + j + dx / 8 + (y_part + i + dy) * wb_bitmap;
      if (pgm)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[idx]);
#else
        data = bitmap[idx];
#endif
      }
      else
      {
        data = bitmap[idx];
      }
      if (invert)
        data = ~data;
      _transfer(data);
    }
  }
  _endTransfer();
  delay(1); // yield() to avoid WDT on ESP8266 and ESP32
}

void GxEPD2_290::__writeImage(const uint8_t *black, const uint8_t *color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    __writeImage(black, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_290::__writeImagePart(const uint8_t *black, const uint8_t *color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                  int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (black)
  {
    __writeImagePart(black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_290::__writeNative(const uint8_t *data1, const uint8_t *data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    __writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_290::__drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  __writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  __refresh(x, y, w, h);
  __writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_290::__drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  __writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  __refresh(x, y, w, h);
  __writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_290::__drawImage(const uint8_t *black, const uint8_t *color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  __writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  __refresh(x, y, w, h);
  __writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_290::__drawNative(const uint8_t *data1, const uint8_t *data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  __writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  __refresh(x, y, w, h);
  __writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_290::__drawImagePart(const uint8_t *black, const uint8_t *color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  __writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  __refresh(x, y, w, h);
  __writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_290::__refresh(bool partial_update_mode)
{
  if (partial_update_mode)
    __refresh(0, 0, WIDTH, HEIGHT);
  else
  {
    if (_using_partial_mode)
      _Init_Full();
    _Update_Full();
    _initial_refresh = false; // initial full update done
  }
}

void GxEPD2_290::__refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  if (_initial_refresh)
    return __refresh(false); // initial update needs be full update
  // intersection with screen
  int16_t w1 = x < 0 ? w + x : w;                             // reduce
  int16_t h1 = y < 0 ? h + y : h;                             // reduce
  int16_t x1 = x < 0 ? 0 : x;                                 // limit
  int16_t y1 = y < 0 ? 0 : y;                                 // limit
  w1 = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1;   // limit
  h1 = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1; // limit
  if ((w1 <= 0) || (h1 <= 0))
    return;
  // make x1, w1 multiple of 8
  w1 += x1 % 8;
  if (w1 % 8 > 0)
    w1 += 8 - w1 % 8;
  x1 -= x1 % 8;
  if (!_using_partial_mode)
    _Init_Part();
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
}

void GxEPD2_290::__powerOff(void)
{
  _PowerOff();
}

void GxEPD2_290::__hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x10); // deep sleep mode
    _writeData(0x1);     // enter deep sleep
    _hibernating = true;
  }
}

void GxEPD2_290::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  _writeCommand(0x11); // set ram entry mode
  _writeData(0x03);    // x increase, y increase : normal mode
  _writeCommand(0x44);
  _writeData(x / 8);
  _writeData((x + w - 1) / 8);
  _writeCommand(0x45);
  _writeData(y % 256);
  _writeData(y / 256);
  _writeData((y + h - 1) % 256);
  _writeData((y + h - 1) / 256);
  _writeCommand(0x4e);
  _writeData(x / 8);
  _writeCommand(0x4f);
  _writeData(y % 256);
  _writeData(y / 256);
}

void GxEPD2_290::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x22);
    _writeData(0xc0);
    _writeCommand(0x20);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_290::_PowerOff()
{
  _writeCommand(0x22);
  _writeData(0xc3);
  _writeCommand(0x20);
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
  _using_partial_mode = false;
}

void GxEPD2_290::_InitDisplay()
{
  if (_hibernating)
    _reset();
  _writeCommand(0x01); // Panel configuration, Gate selection
  _writeData((HEIGHT - 1) % 256);
  _writeData((HEIGHT - 1) / 256);
  _writeData(0x00);
  _writeCommand(0x0c); // softstart
  _writeData(0xd7);
  _writeData(0xd6);
  _writeData(0x9d);
  _writeCommand(0x2c); // VCOM setting
  _writeData(0xa8);    // * different
  _writeCommand(0x3a); // DummyLine
  _writeData(0x1a);    // 4 dummy line per gate
  _writeCommand(0x3b); // Gatetime
  _writeData(0x08);    // 2us per line
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  _writeCommand(0x3C); // BorderWaveform
  _writeData(0x05);
}

const uint8_t GxEPD2_290::LUTDefault_full[] PROGMEM =
    {
        0x32, // command
        0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t GxEPD2_290::LUTDefault_part[] PROGMEM =
    {
        0x32, // command
        0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void GxEPD2_290::_Init_Full()
{
  _InitDisplay();
  _writeCommandDataPGM(LUTDefault_full, sizeof(LUTDefault_full));
  _PowerOn();
  _using_partial_mode = false;
}

void GxEPD2_290::_Init_Part()
{
  _InitDisplay();
  _writeCommandDataPGM(LUTDefault_part, sizeof(LUTDefault_part));
  _PowerOn();
  _using_partial_mode = true;
}

void GxEPD2_290::_Update_Full()
{
  _writeCommand(0x22);
  _writeData(0xc4);
  _writeCommand(0x20);
  _waitWhileBusy("_Update_Full", full_refresh_time);
  _writeCommand(0xff);
}

void GxEPD2_290::_Update_Part()
{
  _writeCommand(0x22);
  _writeData(0x04);
  _writeCommand(0x20);
  _waitWhileBusy("_Update_Part", partial_refresh_time);
  _writeCommand(0xff);
}

////下面实现多线程

enum function_type_t
{
  FUNC_clearScreen,
  FUNC_writeScreenBuffer,
  FUNC_writeImage,
  FUNC_writeImagePart,
  FUNC_writeImage2,
  FUNC_writeImagePart2,
  FUNC_writeNative,
  FUNC_drawImage,
  FUNC_drawImagePart,
  FUNC_drawImage2,
  FUNC_drawImagePart2,
  FUNC_drawNative,
  FUNC_refresh,
  FUNC_refresh2,
  FUNC_powerOff,
  FUNC_hibernate,
};

typedef struct
{
  function_type_t function_type;
  uint8_t value;
  const uint8_t *bitmap;
  const uint8_t *color;
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
  bool invert;
  bool mirror_y;
  bool pgm;
  int16_t x_part;
  int16_t y_part;
  int16_t w_bitmap;
  int16_t h_bitmap;
  bool partial_update_mode;
  bool full_refresh;
  const uint8_t *data1;
  const uint8_t *data2;
  GxEPD2_290 *instance;
} multi_thread_params_t;

static QueueHandle_t multi_thread_queue = NULL;
static bool queue_busy = false;
static void process_multi_thread_queue()
{
  multi_thread_params_t multi_thread_params;
  xQueueReceive(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
  queue_busy = true;
  switch (multi_thread_params.function_type)
  {
  case FUNC_clearScreen:
    multi_thread_params.instance->__clearScreen(multi_thread_params.value);
    break;
  case FUNC_writeScreenBuffer:
    multi_thread_params.instance->__writeScreenBuffer(multi_thread_params.value);
    break;
  case FUNC_writeImage:
    multi_thread_params.instance->__writeImage(multi_thread_params.bitmap, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_writeImagePart:
    multi_thread_params.instance->__writeImagePart(multi_thread_params.bitmap, multi_thread_params.x_part, multi_thread_params.y_part, multi_thread_params.w_bitmap, multi_thread_params.h_bitmap, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_writeImage2:
    multi_thread_params.instance->__writeImage(multi_thread_params.bitmap, multi_thread_params.color, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_writeImagePart2:
    multi_thread_params.instance->__writeImagePart(multi_thread_params.bitmap, multi_thread_params.color, multi_thread_params.x_part, multi_thread_params.y_part, multi_thread_params.w_bitmap, multi_thread_params.h_bitmap, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_writeNative:
    multi_thread_params.instance->__writeNative(multi_thread_params.data1, multi_thread_params.data2, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_drawImage:
    multi_thread_params.instance->__drawImage(multi_thread_params.bitmap, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_drawImagePart:
    multi_thread_params.instance->__drawImagePart(multi_thread_params.bitmap, multi_thread_params.x_part, multi_thread_params.y_part, multi_thread_params.w_bitmap, multi_thread_params.h_bitmap, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_drawImage2:
    multi_thread_params.instance->__drawImage(multi_thread_params.bitmap, multi_thread_params.color, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_drawImagePart2:
    multi_thread_params.instance->__drawImagePart(multi_thread_params.bitmap, multi_thread_params.color, multi_thread_params.x_part, multi_thread_params.y_part, multi_thread_params.w_bitmap, multi_thread_params.h_bitmap, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_drawNative:
    multi_thread_params.instance->__drawNative(multi_thread_params.data1, multi_thread_params.data2, multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h, multi_thread_params.invert, multi_thread_params.mirror_y, multi_thread_params.pgm);
    break;
  case FUNC_refresh:
    multi_thread_params.instance->__refresh(multi_thread_params.partial_update_mode);
    break;
  case FUNC_refresh2:
    multi_thread_params.instance->__refresh(multi_thread_params.x, multi_thread_params.y, multi_thread_params.w, multi_thread_params.h);
    break;
  case FUNC_powerOff:
    multi_thread_params.instance->__powerOff();
    break;
  case FUNC_hibernate:
    multi_thread_params.instance->__hibernate();
    break;
  default:
    break;
  }
}
static void task_gxEPD2_290(void *params)
{
  multi_thread_queue = xQueueCreate(1, sizeof(multi_thread_params_t));
  while (1)
  {
    process_multi_thread_queue();
    queue_busy = false;
  }
}
void GxEPD2_290::startQueue()
{
  xTaskCreate(task_gxEPD2_290, "task_gxEPD2_290", 4096, NULL, 1, NULL);
}
QueueHandle_t GxEPD2_290::getQueue()
{
  return multi_thread_queue;
}
bool GxEPD2_290::isBusy()
{
  return queue_busy;
}
void GxEPD2_290::clearScreen(uint8_t value)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_clearScreen;
  multi_thread_params.value = value;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::writeScreenBuffer(uint8_t value)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_writeScreenBuffer;
  multi_thread_params.value = value;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_writeImage;
  multi_thread_params.bitmap = bitmap;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_writeImagePart;
  multi_thread_params.bitmap = bitmap;
  multi_thread_params.x_part = x_part;
  multi_thread_params.y_part = y_part;
  multi_thread_params.w_bitmap = w_bitmap;
  multi_thread_params.h_bitmap = h_bitmap;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::writeImage(const uint8_t *black, const uint8_t *color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_writeImage2;
  multi_thread_params.bitmap = black;
  multi_thread_params.color = color;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::writeImagePart(const uint8_t *black, const uint8_t *color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_writeImagePart2;
  multi_thread_params.bitmap = black;
  multi_thread_params.color = color;
  multi_thread_params.x_part = x_part;
  multi_thread_params.y_part = y_part;
  multi_thread_params.w_bitmap = w_bitmap;
  multi_thread_params.h_bitmap = h_bitmap;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::writeNative(const uint8_t *data1, const uint8_t *data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_writeNative;
  multi_thread_params.data1 = data1;
  multi_thread_params.data2 = data2;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_drawImage;
  multi_thread_params.bitmap = bitmap;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                               int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_drawImagePart;
  multi_thread_params.bitmap = bitmap;
  multi_thread_params.x_part = x_part;
  multi_thread_params.y_part = y_part;
  multi_thread_params.w_bitmap = w_bitmap;
  multi_thread_params.h_bitmap = h_bitmap;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::drawImage(const uint8_t *black, const uint8_t *color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_drawImage2;
  multi_thread_params.bitmap = black;
  multi_thread_params.color = color;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::drawNative(const uint8_t *data1, const uint8_t *data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_drawNative;
  multi_thread_params.data1 = data1;
  multi_thread_params.data2 = data2;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::drawImagePart(const uint8_t *black, const uint8_t *color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                               int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_drawImagePart2;
  multi_thread_params.bitmap = black;
  multi_thread_params.color = color;
  multi_thread_params.x_part = x_part;
  multi_thread_params.y_part = y_part;
  multi_thread_params.w_bitmap = w_bitmap;
  multi_thread_params.h_bitmap = h_bitmap;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.invert = invert;
  multi_thread_params.mirror_y = mirror_y;
  multi_thread_params.pgm = pgm;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::refresh(bool partial_update_mode)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_refresh;
  multi_thread_params.partial_update_mode = partial_update_mode;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_refresh2;
  multi_thread_params.x = x;
  multi_thread_params.y = y;
  multi_thread_params.w = w;
  multi_thread_params.h = h;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::powerOff(void)
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_powerOff;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}

void GxEPD2_290::hibernate()
{
  multi_thread_params_t multi_thread_params;
  multi_thread_params.function_type = FUNC_hibernate;
  multi_thread_params.instance = this;
  xQueueSend(multi_thread_queue, &multi_thread_params, portMAX_DELAY);
}