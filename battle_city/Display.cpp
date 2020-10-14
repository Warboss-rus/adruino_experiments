#include <Arduino.h>
#include "Display.h"
#include "Pins.h"
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();                   // Invoke custom library with default width and height

void Display::Init()
{
  tft.init();
  tft.setRotation(1);
}

void Display::ClearScreen(uint16_t color)
{
  tft.fillScreen(color);
}

void Display::FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  tft.fillRect(x, y, w, h, color);
}

void Display::DrawSprite(int16_t x, int16_t y, int16_t w, int16_t h, const byte* sprite, uint16_t frontColor, uint16_t backColor)
{
  tft.drawBitmap(x, y, sprite, w, h, frontColor, backColor);
}

void Display::DrawTransparentSprite(int16_t x, int16_t y, int16_t w, int16_t h, const byte* sprite, uint16_t color)
{
  tft.drawBitmap(x, y, sprite, w, h, color);
}

void Display::DrawIndexedSprite(int16_t x, int16_t y, int16_t w, int16_t h, byte* sprite, uint16_t* colorMap)
{
  tft.pushImage(x, y, w, h, sprite, false, colorMap);
}

void Display::DrawText(int16_t x, int16_t y, const char* text, uint16_t color)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.print(text);
}
