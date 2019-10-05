#include "Display.h"
#include <U8g2lib.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, 25, 26, 27);

void Display::init()
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.clearBuffer();
}

void Display::setString(int idx, String str)
{
  u8g2.drawStr(0, idx * 12, str.c_str());
  u8g2.sendBuffer();
}
