#include <Arduino.h>
#include "TankGraphics.h"
#include "Display.h"

namespace
{

const byte TANK_SPRITE_UP_1[] PROGMEM  = {
  B00000000, B00000000,
  B00000001, B00000000,
  B00000001, B00000000,
  B01000001, B00010000,
  B00010001, B00000000,
  B01010101, B00000000,
  B00011100, B00000000,
  B01011011, B00000000,
  B00011010, B00000000,
  B01011010, B00000000,
  B00011100, B00000000,
  B01010110, B00000000,
  B00010000, B00000000,
  B01000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_UP_2[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00110000, B00001100,
  B00000000, B00000000,
  B00100010, B00001100,
  B00000011, B11000000,
  B00100100, B11101100,
  B00000101, B01100000,
  B00100101, B01101100,
  B00000010, B01100000,
  B00100001, B11001100,
  B00000000, B00000000,
  B00110000, B00001100,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_UP_3[] PROGMEM = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B01100000, B00011100,
  B00000000, B11010000,
  B01100000, B00111100,
  B00000000, B00010000,
  B01100000, B10011100,
  B00000000, B10010000,
  B01100001, B10011100,
  B00001000, B00110000,
  B01100111, B11011100,
  B00000000, B00010000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_RIGHT_1[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00011111, B11110000,
  B00000111, B11000000,
  B00011000, B01000000,
  B00010111, B00000000,
  B00000100, B00111110,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00100000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_RIGHT_2[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00101010, B10101000,
  B00100000, B00001000,
  B00001000, B00000000,
  B00000111, B10000000,
  B00001000, B11000000,
  B00011011, B01000000,
  B00011100, B01000000,
  B00010111, B11000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00101010, B10101000,
  B00101010, B10101000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_RIGHT_3[] PROGMEM  = {
  B00000000, B00000000,
  B00111111, B11111000,
  B00010101, B01010000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00100000,
  B00000000, B00100000,
  B00000000, B10000000,
  B00000011, B10100000,
  B00001000, B00100000,
  B00001111, B11000000,
  B00011111, B11111000,
  B00010101, B01010000,
  B00010101, B01010000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_DOWN_1[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00010111, B11000000,
  B01010100, B00000000,
  B00011011, B00000000,
  B01011010, B00000000,
  B00011010, B00000000,
  B01011000, B00000000,
  B00011100, B00000000,
  B01010001, B00000000,
  B00010001, B00000000,
  B01000001, B00000000,
  B00000001, B00000000,
  B00000001, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_DOWN_2[] PROGMEM  = {
  B00000000, B00000000,
  B00110000, B00001100,
  B00000000, B00000000,
  B00101011, B11001100,
  B00000100, B11100000,
  B00100101, B01101100,
  B00000101, B01100000,
  B00100110, B01101100,
  B00000011, B11000000,
  B00100000, B00001100,
  B00000000, B00000000,
  B00110000, B00001100,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_DOWN_3[] PROGMEM  = {
  B00000000, B00000000,
  B01000000, B00010000,
  B01100000, B00011100,
  B00000000, B00110000,
  B01100000, B00011100,
  B00000000, B10100000,
  B01100000, B10011100,
  B00000001, B10010000,
  B01100000, B00111100,
  B00000110, B11010000,
  B01100000, B00011100,
  B00000000, B00010000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_LEFT_1[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001111, B11111000,
  B00000011, B11100000,
  B00000110, B00110000,
  B00000001, B11010000,
  B01111101, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00010000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_LEFT_2[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00010101, B01010100,
  B00010000, B00000000,
  B00000000, B00000000,
  B00000001, B11000000,
  B00000110, B00100000,
  B00000010, B11010000,
  B00000011, B00010000,
  B00000011, B11100000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00010101, B01010100,
  B00010101, B01010100,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte TANK_SPRITE_LEFT_3[] PROGMEM  = {
  B00000000, B00000000,
  B00011111, B11111100,
  B00001010, B10101000,
  B00000000, B00000100,
  B00000000, B00010000,
  B00000000, B00001000,
  B00000000, B00001000,
  B00000000, B00101000,
  B00000100, B11101000,
  B00000100, B00011000,
  B00000011, B11110000,
  B00001111, B11111100,
  B00001010, B10101000,
  B00001010, B10101000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte BRICK_SPRITE[] PROGMEM  = {
  B11110111, B11110111,
  B11110111, B11110111,
  B11110111, B11110111,
  B00000000, B00000000,
  B01111111, B01111111,
  B01111111, B01111111,
  B01111111, B01111111,
  B00000000, B00000000,
  B11110111, B11110111,
  B11110111, B11110111,
  B11110111, B11110111,
  B00000000, B00000000,
  B01111111, B01111111,
  B01111111, B01111111,
  B01111111, B01111111,
  B00000000, B00000000,
};
const byte BRICK_ACCENT_SPRITE[] PROGMEM = {
  B11110111, B11110111,
  B00000100, B00000100,
  B00000100, B00000100,
  B00000000, B00000000,
  B01111111, B01111111,
  B01000000, B01000000,
  B01000000, B01000000,
  B00000000, B00000000,
  B11110111, B11110111,
  B00000100, B00000100,
  B00000100, B00000100,
  B00000000, B00000000,
  B01111111, B01111111,
  B01000000, B01000000,
  B01000000, B01000000,
  B00000000, B00000000,
};
const byte BRICK_SPRITE_SMALL[] PROGMEM = {
  B11110111,
  B11110111,
  B11110111,
  B00000000,
  B01111111,
  B01111111,
  B01111111,
  B00000000,
};
const byte BRICK_ACCENT_SPRITE_SMALL[] PROGMEM = {
  B11110111,
  B00000100,
  B00000100,
  B00000000,
  B01111111,
  B01000000,
  B01000000,
  B00000000,
};
const byte HQ_SPRITE[] PROGMEM  = {
  B00000000, B00000000,
  B11000000, B00000011,
  B01100011, B10000110,
  B11110001, B11001111,
  B01110001, B10001110,
  B11111101, B10111111,
  B00111111, B11111100,
  B01111111, B11111110,
  B00111111, B11111100,
  B00111111, B11111100,
  B00011101, B10111000,
  B00000001, B10000000,
  B00000011, B11000000,
  B00001111, B11110000,
  B00001101, B10110000,
  B00000000, B00000000,
};
const byte HQ_ACCENT_SPRITE[] PROGMEM = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B10000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00010000, B00001000,
  B00001000, B00010000,
  B00000010, B01000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte ARMOUR_SPRITE[] PROGMEM  = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00111100, B00111100,
  B00111100, B00111100,
  B00111100, B00111100,
  B00111100, B00111100,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00111100, B00111100,
  B00111100, B00111100,
  B00111100, B00111100,
  B00111100, B00111100,
  B00000000, B00000000,
  B00000000, B00000000,
};
const byte ARMOUR_ACCENT_SPRITE[] PROGMEM = {
  B00000000, B00000000,
  B00000001, B00000001,
  B00000011, B00000011,
  B00000011, B00000011,
  B00000011, B00000011,
  B00000011, B00000011,
  B00111111, B00111111,
  B01111111, B01111111,
  B00000000, B00000000,
  B00000001, B00000001,
  B00000011, B00000011,
  B00000011, B00000011,
  B00000011, B00000011,
  B00000011, B00000011,
  B00111111, B00111111,
  B01111111, B01111111,
};
const byte ARMOUR_SPRITE_SMALL[] PROGMEM  = {
  B00000000,
  B00000000,
  B00111100,
  B00111100,
  B00111100,
  B00111100,
  B00000000,
  B00000000,
};
const byte ARMOUR_ACCENT_SPRITE_SMALL[] PROGMEM = {
  B00000000,
  B00000001,
  B00000011,
  B00000011,
  B00000011,
  B00000011,
  B00111111,
  B01111111,
};
const byte WATER_SPRITE[] PROGMEM = {
  B00000101, B00000101,
  B01000000, B01000000,
  B10100000, B10100000,
  B01011010, B01011010,
  B00010101, B00010101,
  B00010000, B00010000,
  B01101000, B01101000,
  B10100010, B10100010,
  B00000101, B00000101,
  B01000000, B01000000,
  B10100000, B10100000,
  B01011010, B01011010,
  B00010101, B00010101,
  B00010000, B00010000,
  B01101000, B01101000,
  B10100010, B10100010,
};
const byte WATER_ACCENT_SPRITE[] PROGMEM = {
  B00000001, B00000001,
  B01000000, B01000000,
  B00100000, B00100000,
  B00010010, B00010010,
  B00000001, B00000001,
  B00010000, B00010000,
  B00101000, B00101000,
  B10000000, B10000000,
  B00000001, B00000001,
  B01000000, B01000000,
  B00100000, B00100000,
  B00010010, B00010010,
  B00000001, B00000001,
  B00010000, B00010000,
  B00101000, B00101000,
  B10000000, B10000000,
};
const byte WATER_SPRITE_SMALL[] PROGMEM = {
  B00000101,
  B01000000,
  B10100000,
  B01011010,
  B00010101,
  B00010000,
  B01101000,
  B10100010,
};
const byte WATER_ACCENT_SPRITE_SMALL[] PROGMEM = {
  B00000001,
  B01000000,
  B00100000,
  B00010010,
  B00000001,
  B00010000,
  B00101000,
  B10000000,
};
const byte TREE_SPRITE[] PROGMEM = {
  B00000010, B00000010,
  B00010101, B00010101,
  B00000111, B00000111,
  B00011001, B00011001,
  B00101110, B00101110,
  B00011111, B00011111,
  B11111011, B11111011,
  B01101110, B01101110,
  B00000010, B00000010,
  B00010101, B00010101,
  B00000111, B00000111,
  B00011001, B00011001,
  B00101110, B00101110,
  B00011111, B00011111,
  B11111011, B11111011,
  B01101110, B01101110,
};
const byte TREE_SPRITE_SMALL[] PROGMEM = {
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
  B00000000, B00000000,
  B01000100, B10000100,
  B00011001, B10001000,
  B00001001, B00011000,
  B00000111, B10110000,
  B01001101, B01100000,
  B00011000, B00001000,
  B11110100, B00111111,
  B00001100, B00100000,
  B00000001, B00001100,
  B01001101, B01010010,
  B00111010, B10110000,
  B00100101, B10011000,
  B01000000, B10000100,
  B10000000, B10000010,
  B00000000, B10000000,
};
const byte EXPLOSION_BACK[] PROGMEM = {
  B00001010, B00010010,
  B00000000, B00100010,
  B01100000, B00000100,
  B00110110, B11000100,
  B00010000, B01001101,
  B00010000, B00001000,
  B00100010, B00010100,
  B00000000, B00000000,
  B01110010, B00011100,
  B00011100, B00110000,
  B00110010, B00001100,
  B00000001, B01000000,
  B01011010, B01100010,
  B00100101, B01011000,
  B01001000, B00000100,
  B00000000, B00100010,
};
const byte EXPLOSION_ACCENT[] PROGMEM = {
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00001000, B00000000,
  B00000010, B00010000,
  B00000101, B10100000,
  B00000010, B01000000,
  B00000001, B01000000,
  B00000000, B10000000,
  B00000000, B00100000,
  B00000100, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
  B00000000, B00000000,
};

const uint16_t COLOR_BLACK = 0x0; //0x000000
const uint16_t BRICK_FRONT_COLOR = 0x9A40; //0x9c4a00
const uint16_t BRICK_BACK_COLOR = 0x630C; //0x636363
const uint16_t BRICK_ACCENT_COLOR = 0x6840; //0x6B0800
const uint16_t HQ_COLOR = 0x630C; //0x636363
const uint16_t HQ_ACCENT_COLOR = 0x6840; //0x6B0800
const uint16_t ARMOUR_FRONT_COLOR = 0xFFFF; //0xFFFFFF
const uint16_t ARMOUR_BACK_COLOR = 0xAD75; //0xADADAD
const uint16_t ARMOUR_ACCENT_COLOR = 0x630C; //0x636363
const uint16_t BULLET_COLOR = 0xAD75; //0xadadad
const uint16_t WATER_FRONT_COLOR = 0xB77D; //0xb5efef
const uint16_t WATER_BACK_COLOR = 0x421F; //0x4242ff
const uint16_t WATER_ACCENT_COLOR = 0xAD75; //0xADADAD;
const uint16_t TREE_FRONT_COLOR = 0x8EA0; //0x8cd600
const uint16_t TREE_BACK_COLOR = 0x0281; //0x005208
const uint16_t EXPLOSION_FRONT_COLOR = 0xFFFF; //0xFFFFFF
const uint16_t EXPLOSION_BACK_COLOR = 0x587A; //0x590dd4
const uint16_t EXPLOSION_ACCENT_COLOR = 0xB184; //0xb53121
const uint16_t TANK_COLOR1[] = {0xFFFF, 0xE732, 0xB7B9, 0xFFFF}; // 0xFFFFFF, 0xE7E794, 0xB5F7CE, 0xFFFFFF
const uint16_t TANK_COLOR2[] = {0xAD75, 0xE4E4, 0x0466, 0xB184}; // 0xADADAD, 0xE79C21, 0x008C31, 0xB53121
const uint16_t TANK_COLOR3[] = {0x0024, 0x6B40, 0x0280, 0x580F}; //0x00424A, 0x6B6B00, 0x005200, 0x5A007B
const uint16_t TEXT_COLOR = 0xB184; //0xb53121, 0x005200

const byte* getTankSprite(tank::Direction dir, int idx)
{
  switch (dir)
  {
    case tank::DIR_UP: {
        const byte* arr[] = {TANK_SPRITE_UP_1, TANK_SPRITE_UP_2, TANK_SPRITE_UP_3};
        return arr[idx];
      }
    case tank::DIR_LEFT: {
        const byte* arr[] = {TANK_SPRITE_LEFT_1, TANK_SPRITE_LEFT_2, TANK_SPRITE_LEFT_3};
        return arr[idx];
      }
    case tank::DIR_RIGHT: {
        const byte* arr[] = {TANK_SPRITE_RIGHT_1, TANK_SPRITE_RIGHT_2, TANK_SPRITE_RIGHT_3};
        return arr[idx];
      }
    case tank::DIR_DOWN: {
        const byte* arr[] = {TANK_SPRITE_DOWN_1, TANK_SPRITE_DOWN_2, TANK_SPRITE_DOWN_3};
        return arr[idx];
      }
  }
}

}

namespace tank
{

void Graphics::DrawSprite(ushort x, ushort y, const byte* sprite, uint16_t frontColor, uint16_t backColor)
{
  Display::DrawSprite(x, y, SPRITE_SIZE, SPRITE_SIZE, sprite, frontColor, backColor);
}

void Graphics::DrawTransparentSprite(ushort x, ushort y, const byte* sprite, uint16_t color)
{
  Display::DrawTransparentSprite(x, y, SPRITE_SIZE, SPRITE_SIZE, sprite, color);
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

void Graphics::DrawBricks(ushort x, ushort y)
{
  Graphics::DrawSprite(x, y, BRICK_SPRITE, BRICK_FRONT_COLOR, BRICK_BACK_COLOR);
  Graphics::DrawTransparentSprite(x, y, BRICK_ACCENT_SPRITE, BRICK_ACCENT_COLOR);
}

void Graphics::DrawArmour(ushort x, ushort y)
{
  Graphics::DrawSprite(x, y, ARMOUR_SPRITE, ARMOUR_FRONT_COLOR, ARMOUR_BACK_COLOR);
  Graphics::DrawTransparentSprite(x, y, ARMOUR_ACCENT_SPRITE, ARMOUR_ACCENT_COLOR);
}

void Graphics::DrawTrees(ushort x, ushort y)
{
  Graphics::DrawSprite(x, y, TREE_SPRITE, TREE_FRONT_COLOR, TREE_BACK_COLOR);
}

void Graphics::DrawWater(ushort x, ushort y)
{
  Graphics::DrawSprite(x, y, WATER_SPRITE, WATER_FRONT_COLOR, WATER_BACK_COLOR);
  Graphics::DrawTransparentSprite(x, y, WATER_ACCENT_SPRITE, WATER_ACCENT_COLOR);
}

void Graphics::DrawHQ(ushort x, ushort y)
{
  Graphics::DrawTransparentSprite(x, y, HQ_SPRITE, HQ_COLOR);
  Graphics::DrawTransparentSprite(x, y, HQ_ACCENT_SPRITE, HQ_ACCENT_COLOR);
}

void Graphics::DrawTank(Point pos, Direction dir, TankIndex index)
{
  Graphics::DrawSprite(pos.x, pos.y, getTankSprite(dir, 0), TANK_COLOR1[index], COLOR_BLACK);
  Graphics::DrawTransparentSprite(pos.x, pos.y, getTankSprite(dir, 1), TANK_COLOR2[index]);
  Graphics::DrawTransparentSprite(pos.x, pos.y, getTankSprite(dir, 2), TANK_COLOR3[index]);

}

void Graphics::DrawExplosion(ushort x, ushort y, byte frame)
{
  Graphics::DrawSprite(x, y, EXPLOSION_BACK, EXPLOSION_BACK_COLOR, COLOR_BLACK);
  Graphics::DrawTransparentSprite(x, y, EXPLOSION_FRONT, EXPLOSION_FRONT_COLOR);
  Graphics::DrawTransparentSprite(x, y, EXPLOSION_ACCENT, EXPLOSION_ACCENT_COLOR);
}

void Graphics::DrawText(ushort x, ushort y, const char* text)
{
  Display::DrawText(x, y, text, TEXT_COLOR);
}

}
