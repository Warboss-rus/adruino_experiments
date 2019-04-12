#include "Arduino.h"
#include "Graphics.h"
#include <Adafruit_GFX.h> // core graphics library
#include <Adafruit_ST7735.h> // hardware-specific library

Adafruit_ST7735 tft(PB5, PB3, PB4);

const byte TANK_SPRITE[] PROGMEM  = {
  B00001000,
  B11001011,
  B01001010,
  B11111111,
  B01111110,
  B11111111,
  B01111110,
  B11011011,
};
const byte TANK_SPRITE_RIGHT[] PROGMEM  = {
  B10101010,
  B11111110,
  B01111000,
  B11111000,
  B11111111,
  B01111000,
  B11111110,
  B10101010,
};
const byte TANK_SPRITE_DOWN[] PROGMEM  = {
  B11011011,
  B01111110,
  B11111111,
  B01111110,
  B11111111,
  B01010010,
  B11010011,
  B00010000,
};
const byte TANK_SPRITE_LEFT[] PROGMEM  = {
  B01010101,
  B01111111,
  B00011110,
  B11111111,
  B00011111,
  B00011110,
  B01111111,
  B01010101,
};
const byte BRICK_SPRITE[] PROGMEM  = {
  B10111011,
  B10111011,
  B00000000,
  B11101110,
  B11101110,
  B00000000,
  B10111011,
  B10111011,
};
const byte HQ_SPRITE[] PROGMEM  = {
  B11011011,
  B01010010,
  B01111110,
  B00111100,
  B00111100,
  B00011000,
  B00111100,
  B01011010,
};
const byte ARMOUR_SPRITE[] PROGMEM  = {
  B11101110,
  B11101110,
  B11101110,
  B00000000,
  B11101110,
  B11101110,
  B11101110,
  B00000000,
};
const byte WATER_SPRITE[] PROGMEM = {
  B11111110,
  B10111111,
  B11011111,
  B11101101,
  B11111110,
  B11101111,
  B11010111,
  B01111111,
};
const byte TREE_SPRITE[] PROGMEM = {
  B00000010,
  B00010101,
  B00000111,
  B00011001,
  B00101110,
  B00011111,
  B11111011,
  B01101110,
};
const byte EXPLOSION_FRONT[] PROGMEM = {
  B11000001,
  B10010010,
  B01111100,
  B11100111,
  B00110100,
  B00011100,
  B01001010,
  B10001001,
};
const byte EXPLOSION_BACK[] PROGMEM = {
  B00010010,
  B00100100,
  B10000001,
  B00000000,
  B01000001,
  B00100010,
  B00010100,
  B01000000,
};
const byte EXPLISION_ACCENT[] PROGMEM = {
  B00000000,
  B01000000,
  B00000010,
  B00010000,
  B00001000,
  B00000000,
  B00000000,
  B00000000,
};

const uint16_t BRICK_FRONT_COLOR = 0x9A40; //0x9c4a00
const uint16_t BRICK_BACK_COLOR = 0x630C; //0x636363
const uint16_t P1_TANK_COLOR = 0xE4E4; //0xe79c21
const uint16_t P2_TANK_COLOR = 0x0466; //0x008c31
const uint16_t ENEMY_TANK_COLOR = 0xAD75; //0xadadad
const uint16_t HQ_COLOR = 0x630C; //0x636363
const uint16_t ARMOUR_FRONT_COLOR = 0xFFFF; //0xFFFFFF
const uint16_t ARMOUR_BACK_COLOR = 0x630C; //0x636363
const uint16_t BULLET_COLOR = 0xAD75; //0xadadad
const uint16_t WATER_FRONT_COLOR = 0x421F; //0x4242ff
const uint16_t WATER_BACK_COLOR = 0xB77D; //0xb5efef
const uint16_t TREE_FRONT_COLOR = 0x8EA0; //0x8cd600
const uint16_t TREE_BACK_COLOR = 0x0281; //0x005208
const uint16_t EXPLOSION_FRONT_COLOR = 0xFFFF; //0xFFFFFF
const uint16_t EXPLOSION_BACK_COLOR = 0x587A; //0x590dd4
const uint16_t EXPLOSION_ACCENT_COLOR = 0xB184; //0xb53121
const uint16_t TANK_COLORS[] = {ENEMY_TANK_COLOR, P1_TANK_COLOR, P2_TANK_COLOR};
const uint16_t TEXT_COLOR = 0xB184; //0xb53121

const byte* getTankSprite(Direction dir)
{
  switch (dir)
  {
    case DIR_UP:
      return TANK_SPRITE;
    case DIR_DOWN:
      return TANK_SPRITE_DOWN;
    case DIR_LEFT:
      return TANK_SPRITE_LEFT;
    case DIR_RIGHT:
      return TANK_SPRITE_RIGHT;
  }
  return NULL;
}

void drawSprite(byte x, byte y, const byte* sprite, uint16_t frontColor, uint16_t backColor)
{
  tft.drawBitmap(x, y, sprite, SPRITE_SIZE, SPRITE_SIZE, frontColor, backColor);
}

void drawTransparentSprite(byte x, byte y, const byte* sprite, uint16_t color)
{
  tft.drawBitmap(x, y, sprite, SPRITE_SIZE, SPRITE_SIZE, color);
}

void Graphics::InitScreen()
{
  tft.initR(INITR_BLACKTAB);
}

void Graphics::ClearScreen()
{
  tft.fillScreen(ST7735_BLACK);
}

void Graphics::ClearSprite(byte x, byte y)
{
  tft.fillRect(x, y, SPRITE_SIZE, SPRITE_SIZE, ST7735_BLACK);
}

void Graphics::DrawBullet(byte x, byte y)
{
  tft.fillRect(x - BULLET_SIZE / 2, y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE, BULLET_COLOR);
}

void Graphics::ClearBullet(byte x, byte y)
{
  tft.fillRect(x - BULLET_SIZE / 2, y - BULLET_SIZE / 2, BULLET_SIZE, BULLET_SIZE, ST7735_BLACK);
}

void Graphics::DrawBricks(byte x, byte y)
{
  drawSprite(x, y, BRICK_SPRITE, BRICK_FRONT_COLOR, BRICK_BACK_COLOR);
}

void Graphics::DrawArmour(byte x, byte y)
{
  drawSprite(x, y, ARMOUR_SPRITE, ARMOUR_FRONT_COLOR, ARMOUR_BACK_COLOR);
}

void Graphics::DrawTrees(byte x, byte y)
{
  drawSprite(x, y, TREE_SPRITE, TREE_FRONT_COLOR, TREE_BACK_COLOR);
}

void Graphics::DrawWater(byte x, byte y)
{
  drawSprite(x, y, WATER_SPRITE, WATER_FRONT_COLOR, WATER_BACK_COLOR);
}

void Graphics::DrawHQ(byte x, byte y)
{
  drawTransparentSprite(x, y, HQ_SPRITE, HQ_COLOR);
}

void Graphics::DrawTank(byte x, byte y, Direction dir, TankIndex index)
{
  drawTransparentSprite(x, y, getTankSprite(dir), TANK_COLORS[index]);
}

void Graphics::DrawExplosion(byte x, byte y, byte frame)
{
  drawSprite(x, y, EXPLOSION_BACK, EXPLOSION_BACK_COLOR, ST7735_BLACK);
  drawTransparentSprite(x,y, EXPLOSION_FRONT, EXPLOSION_FRONT_COLOR);
  drawTransparentSprite(x,y, EXPLISION_ACCENT, EXPLOSION_ACCENT_COLOR);
}

void Graphics::DrawText(byte x, byte y, const char* text)
{
  tft.setCursor(x, y);
  tft.setTextColor(TEXT_COLOR);
  tft.print(text);
}
