#include <Arduino.h>
#include "Display.h"
#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library
#include "Pins.h"

Adafruit_ST7735 tft(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);

void Display::Init()
{
  tft.initR(INITR_BLACKTAB);
}

void Display::ClearScreen(uint16_t color)
{
  tft.fillScreen(ST7735_BLACK);
}

void Display::FillRect(byte x, byte y, byte w, byte h, uint16_t color)
{
  tft.fillRect(x, y, w, h, color);
}

void Display::DrawSprite(byte x, byte y, byte w, byte h, const byte* sprite, uint16_t frontColor, uint16_t backColor)
{
  tft.drawBitmap(x, y, sprite, w, h, frontColor, backColor);
}

void Display::DrawTransparentSprite(byte x, byte y, byte w, byte h, const byte* sprite, uint16_t color)
{
  tft.drawBitmap(x, y, sprite, w, h, color);
}

void Display::DrawText(byte x, byte y, const char* text, uint16_t color)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.print(text);
}
