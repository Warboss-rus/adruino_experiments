#include <Arduino.h>
#include "Graphics.h"
#include "Display.h"

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

const uint16_t COLOR_BLACK = 0x0; //0x000000
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

void Graphics::DrawSprite(byte x, byte y, const byte* sprite, uint16_t frontColor, uint16_t backColor)
{
  Display::DrawSprite(x, y, SPRITE_SIZE, SPRITE_SIZE, sprite, frontColor, backColor);
}

void Graphics::DrawTransparentSprite(byte x, byte y, const byte* sprite, uint16_t color)
{
  Display::DrawTransparentSprite(x, y, SPRITE_SIZE, SPRITE_SIZE, sprite, color);
}

void Graphics::InitScreen()
{
  Display::Init();
}

void Graphics::ClearScreen()
{
  Display::ClearScreen(COLOR_BLACK);
}

void Graphics::ClearSprite(Point pos)
{
  Display::FillRect(pos.x, pos.y, SPRITE_SIZE, SPRITE_SIZE, COLOR_BLACK);
}

void Graphics::ClearMovingSprite(Point oldPos, Point newPos)
{
  if (newPos.x > oldPos.x)
  {
    Display::FillRect(oldPos.x, oldPos.y, newPos.x - oldPos.x, SPRITE_SIZE, COLOR_BLACK);
  }
  if (newPos.x < oldPos.x)
  {
    Display::FillRect(newPos.x + SPRITE_SIZE, oldPos.y, oldPos.x - newPos.x, SPRITE_SIZE, COLOR_BLACK);
  }
  if (newPos.y > oldPos.y)
  {
    Display::FillRect(oldPos.x, oldPos.y, SPRITE_SIZE, newPos.y - oldPos.y, COLOR_BLACK);
  }
  if (newPos.y < oldPos.y)
  {
    Display::FillRect(oldPos.x, newPos.y + SPRITE_SIZE, SPRITE_SIZE, oldPos.y - newPos.y, COLOR_BLACK);
  }
}

void Graphics::DrawBullet(Point pos)
{
  Display::FillRect(pos.x, pos.y, BULLET_SIZE, BULLET_SIZE, BULLET_COLOR);
}

void Graphics::ClearBullet(Point pos)
{
  Display::FillRect(pos.x, pos.y, BULLET_SIZE, BULLET_SIZE, COLOR_BLACK);
}

void Graphics::DrawBricks(byte x, byte y)
{
  Graphics::DrawSprite(x, y, BRICK_SPRITE, BRICK_FRONT_COLOR, BRICK_BACK_COLOR);
}

void Graphics::DrawArmour(byte x, byte y)
{
  Graphics::DrawSprite(x, y, ARMOUR_SPRITE, ARMOUR_FRONT_COLOR, ARMOUR_BACK_COLOR);
}

void Graphics::DrawTrees(byte x, byte y)
{
  Graphics::DrawSprite(x, y, TREE_SPRITE, TREE_FRONT_COLOR, TREE_BACK_COLOR);
}

void Graphics::DrawWater(byte x, byte y)
{
  Graphics::DrawSprite(x, y, WATER_SPRITE, WATER_FRONT_COLOR, WATER_BACK_COLOR);
}

void Graphics::DrawHQ(byte x, byte y)
{
  Graphics::DrawTransparentSprite(x, y, HQ_SPRITE, HQ_COLOR);
}

void Graphics::DrawTank(Point pos, Direction dir, TankIndex index)
{
  Graphics::DrawSprite(pos.x, pos.y, getTankSprite(dir), TANK_COLORS[index], COLOR_BLACK);
}

void Graphics::DrawExplosion(byte x, byte y, byte frame)
{
  Graphics::DrawSprite(x, y, EXPLOSION_BACK, EXPLOSION_BACK_COLOR, COLOR_BLACK);
  Graphics::DrawTransparentSprite(x, y, EXPLOSION_FRONT, EXPLOSION_FRONT_COLOR);
  Graphics::DrawTransparentSprite(x, y, EXPLISION_ACCENT, EXPLOSION_ACCENT_COLOR);
}

void Graphics::DrawText(byte x, byte y, const char* text)
{
  Display::DrawText(x, y, text, TEXT_COLOR);
}
